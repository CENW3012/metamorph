#ifndef MONOLOGUE_H
#define MONOLOGUE_H

#include "dialogue.h"

/* ── Limits ───────────────────────────────────────────────────────────── */

#define MONOLOGUE_ID_MAX        64
#define MONOLOGUE_SPEAKER_MAX   64
#define MONOLOGUE_TEXT_MAX    2048
#define MAX_MONOLOGUE_SECTIONS  64

/* ── Data structures ──────────────────────────────────────────────────── */

typedef struct {
    char id[MONOLOGUE_ID_MAX];
    char speaker[MONOLOGUE_SPEAKER_MAX];
    char text[MONOLOGUE_TEXT_MAX];
} MonologueSection;

typedef struct {
    MonologueSection sections[MAX_MONOLOGUE_SECTIONS];
    int              count;
} MonologueFile;

/* ── API ──────────────────────────────────────────────────────────────── */

/* Load all sections from a .txt file. Returns 1 on success, 0 on failure. */
int monologue_load(MonologueFile *mf, const char *path);

/* Find a section by its ID string. Returns NULL when not found. */
const MonologueSection *monologue_find(const MonologueFile *mf,
                                       const char *id);

/* Build a DialogueTree from a section. Long text is automatically split
 * into chained nodes so each node fits within DIALOGUE_TEXT_MAX chars. */
DialogueTree *monologue_to_dialogue_tree(const MonologueSection *section);

#endif /* MONOLOGUE_H */
