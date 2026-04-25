#ifndef INPUT_H
#define INPUT_H

typedef struct {
    int forward;
    int backward;
    int strafe_left;
    int strafe_right;
    int turn_left;
    int turn_right;
} PlayerInput;

PlayerInput *input_get_current(void);
int input_consume_ready_toggle(void);
int input_consume_character_prev(void);
int input_consume_character_next(void);
void input_init_callbacks(void);
void input_apply_forces(void);

#endif