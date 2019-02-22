#ifndef _Motor_H_
#define _Motor_H_

void Forward(void);
void Backward(void);
void TurnRight(void);
void TurnLeft(void);
void Stop(void);
void GPIO_Ctrl_InitDirPin();
void Init_PWM_CLOCK(void);
void Init_PWM(void);

#endif /* _Motor_H_ */
