#include "alt_main.hpp"

class PwmOut {
  public:
    PwmOut(TIM_HandleTypeDef *_htim, uint32_t channel1, uint32_t channel2, uint32_t channel3) {
        htim = _htim;
        channel[0] = channel1;
        channel[1] = channel2;
        channel[2] = channel3;
    }
    void write(float duty1, float duty2, float duty3) {
        duty1 = duty1 > 1.0 ? 1 : duty1;
        duty1 = duty1 < 0.0 ? 0 : duty1;

        duty2 = duty2 > 1.0 ? 1 : duty2;
        duty2 = duty2 < 0.0 ? 0 : duty2;

        duty3 = duty3 > 1.0 ? 1 : duty3;
        duty3 = duty3 < 0.0 ? 0 : duty3;

        duty1 = (int)(duty1 * maxValue);
        duty2 = (int)(duty2 * maxValue);
        duty3 = (int)(duty3 * maxValue);

        __HAL_TIM_SET_COMPARE(htim, channel[0], duty1);
        __HAL_TIM_SET_COMPARE(htim, channel[1], duty2);
        __HAL_TIM_SET_COMPARE(htim, channel[2], duty3);
    }

    void init() {
        maxValue = htim->Init.Period;
        HAL_TIM_PWM_Start(htim, channel[0]);
        HAL_TIM_PWM_Start(htim, channel[1]);
        HAL_TIM_PWM_Start(htim, channel[2]);
        printf("init PWM!! MaxCount:%d\n", htim->Init.Period);
    }

  private:
    TIM_HandleTypeDef *htim;
    uint32_t channel[3];
    uint32_t maxValue;
};

PwmOut pwm(&htim1, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3);

int alt_main() {
    pwm.init();
    int deg = 0;
    while (1) {
        deg++;
        float u = 0.5 * sinDeg(deg) + 0.5;
        float v = 0.5 * sinDeg(deg + 120) + 0.5;
        float w = 0.5 * sinDeg(deg + 240) + 0.5;
        pwm.write(u, v, w);
        HAL_Delay(10);
        printf("%d %d %d\n", (int)(u * 1000), (int)(v * 1000), (int)(w * 1000));
    }
}