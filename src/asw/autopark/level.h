#ifndef ASW_AUTOPARK_LEVEL_H_
#define ASW_AUTOPARK_LEVEL_H_

typedef enum leveldir{
    LEVEL_LEFT, LEVEL_RIGHT
} LevelDir;

void levelInit (LevelDir dir);
int steer(LevelDir dir);

void upKp(int n, float i);

#endif /* ASW_AUTOPARK_LEVEL_H_ */
