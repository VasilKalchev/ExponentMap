linearPWM_plot = subplot(2, 2, 1);
linearPerception_plot = subplot(2, 2, 2);
exponentialPWM_plot = subplot(2, 2, 3);
exponentialPerception_plot = subplot(2, 2, 4);

time = 0:10;

linearPWM = time * 25.5;
linearPerception = log10(time) * 255;

step = time/12;
exponentialPWM = 2.^(time/1.25);
exponentialPerception = log10(exponentialPWM) * 100;

plot(linearPWM_plot, time, linearPWM, 'g');
title(linearPWM_plot, 'Linearly growing PWM');
xlabel(linearPWM_plot, 'time');
ylabel(linearPWM_plot, 'PWM');

plot(linearPerception_plot, linearPerception, 'g');
title(linearPerception_plot, 'Perception of linearly growing PWM');
xlabel(linearPerception_plot, 'time');
ylabel(linearPerception_plot, 'Perceived PWM');

plot(exponentialPWM_plot, time, exponentialPWM);
title(exponentialPWM_plot, 'Exponentially growing PWM');
xlabel(exponentialPWM_plot, 'time');
ylabel(exponentialPWM_plot, 'PWM');

plot(exponentialPerception_plot, time, exponentialPerception);
title(exponentialPerception_plot, 'Perception of exponentially growing PWM');
xlabel(exponentialPerception_plot, 'time');
ylabel(exponentialPerception_plot, 'Perceived PWM');
