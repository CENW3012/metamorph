#include "monologue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Maximum number of characters read per line from the monologue .txt file. */
#define MAX_LINE_LENGTH 512

/* ── Internal helpers ──────────────────────────────────────────────────── */

/* Remove leading and trailing whitespace in-place. */
static void trim(char *s)
{
    if (!s) return;
    /* Leading */
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    /* Trailing */
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end - 1))) end--;
    *end = '\0';
}

/* ── File parser ───────────────────────────────────────────────────────── */

/*
 * Expected .txt format:
 *
 *   [SECTION_ID]
 *   CHARACTER_NAME
 *   ---
 *   Dialogue text here.
 *   Can span multiple lines.
 *   ---
 *
 * Lines beginning with '#' are comments and are ignored.
 * Blank lines outside the text block are ignored.
 * Text lines inside the --- delimiters are joined with a space.
 */
int monologue_load(MonologueFile *mf, const char *path)
{
    if (!mf || !path) return 0;
    memset(mf, 0, sizeof(MonologueFile));

    FILE *f = fopen(path, "r");
    if (!f) {
        SDL_Log("monologue: cannot open '%s'", path);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    MonologueSection *cur     = NULL; /* section currently being built  */
    int               in_text = 0;   /* 1 while inside --- delimiters  */
    int               got_spk = 0;   /* 1 once speaker line was stored */

    while (fgets(line, sizeof(line), f)) {
        /* Strip trailing newline / carriage-return */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[--len] = '\0';
        if (len > 0 && line[len - 1] == '\r') line[--len] = '\0';

        /* Comment lines are always skipped */
        if (line[0] == '#') continue;

        /* ── Section header: [SECTION_ID] ── */
        if (!in_text && line[0] == '[') {
            char *close = strchr(line + 1, ']');
            if (close && mf->count < MAX_MONOLOGUE_SECTIONS) {
                cur = &mf->sections[mf->count++];
                memset(cur, 0, sizeof(MonologueSection));

                size_t id_len = (size_t)(close - (line + 1));
                if (id_len >= MONOLOGUE_ID_MAX)
                    id_len = MONOLOGUE_ID_MAX - 1;
                memcpy(cur->id, line + 1, id_len);
                cur->id[id_len] = '\0';

                got_spk = 0;
                in_text = 0;
            }
            continue;
        }

        if (!cur) continue;

        /* ── "---" delimiter toggles text-reading mode ── */
        if (strcmp(line, "---") == 0) {
            if (!in_text) {
                in_text = 1;        /* entering text block */
            } else {
                in_text = 0;        /* leaving text block  */
                trim(cur->text);
            }
            continue;
        }

        /* ── Speaker name (first non-blank line after the section header) ── */
        if (!in_text && !got_spk) {
            trim(line);
            if (line[0] != '\0') {
                strncpy(cur->speaker, line, MONOLOGUE_SPEAKER_MAX - 1);
                cur->speaker[MONOLOGUE_SPEAKER_MAX - 1] = '\0';
                got_spk = 1;
            }
            continue;
        }

        /* ── Text lines (inside --- delimiters) ── */
        if (in_text) {
            size_t cur_len = strlen(cur->text);

            /* Join with a space unless this is the very first line */
            if (cur_len > 0 && cur_len < (size_t)(MONOLOGUE_TEXT_MAX - 2)) {
                cur->text[cur_len++] = ' ';
                cur->text[cur_len]   = '\0';
            }

            strncat(cur->text, line,
                    (size_t)(MONOLOGUE_TEXT_MAX - strlen(cur->text) - 1));
        }
    }

    fclose(f);
    return mf->count > 0 ? 1 : 0;
}

/* ── Section lookup ────────────────────────────────────────────────────── */

const MonologueSection *monologue_find(const MonologueFile *mf,
                                       const char *id)
{
    if (!mf || !id) return NULL;
    for (int i = 0; i < mf->count; i++) {
        if (strcmp(mf->sections[i].id, id) == 0)
            return &mf->sections[i];
    }
    return NULL;
}

/* ── DialogueTree builder ──────────────────────────────────────────────── */

/*
 * Converts a monologue section into a DialogueTree that uses the existing
 * visual dialogue system.  Text longer than DIALOGUE_TEXT_MAX-1 characters
 * is automatically split into chained nodes; each non-final node has a
 * single "..." choice that advances to the next node.  The last node is
 * marked terminal so ENTER closes the dialogue.
 */
DialogueTree *monologue_to_dialogue_tree(const MonologueSection *section)
{
    if (!section) return NULL;

    DialogueTree *tree = dialogue_tree_create();
    if (!tree) return NULL;

    const char *text    = section->text;
    const char *speaker = section->speaker;
    const int   chunk   = DIALOGUE_TEXT_MAX - 1; /* max chars per node (reserve 1 for '\0') */
    int         total   = (int)strlen(text);
    int         node_id = 0;

    /* Empty section: single terminal placeholder node */
    if (total == 0) {
        dialogue_add_node(tree, 0, speaker, "...", 1);
        return tree;
    }

    int offset = 0;
    while (offset < total) {
        int remaining = total - offset;
        int take = (remaining <= chunk) ? remaining : chunk;

        /* When splitting, prefer to break at the last space within the chunk */
        if (take < remaining) {
            int j = take;
            while (j > 0 && text[offset + j] != ' ') j--;
            if (j > 0) take = j;
        }

        char buf[DIALOGUE_TEXT_MAX];
        memcpy(buf, text + offset, (size_t)take);
        buf[take] = '\0';
        trim(buf);

        int is_last = (offset + take >= total);

        DialogueNode *node = dialogue_add_node(tree, node_id, speaker,
                                               buf, is_last ? 1 : 0);

        /* Non-terminal nodes get a single "..." advance choice */
        if (!is_last && node) {
            DialogueChoice adv;
            memset(&adv, 0, sizeof(adv));
            adv.id           = 0;
            adv.next_node_id = node_id + 1;
            strncpy(adv.text, "...", DIALOGUE_TEXT_MAX - 1);
            dialogue_add_choice(node, &adv);
        }

        /* Advance past the chunk and skip any leading space */
        offset += take;
        while (offset < total && text[offset] == ' ') offset++;
        node_id++;
    }

    return tree;
}
