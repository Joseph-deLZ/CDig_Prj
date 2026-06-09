figure(1); clf, hold off

para2nl  % p.o.
para2nl_PI  
PrPara2nl_PI

subplot(211)
stairs(y.time,y.signals.values(:,1)), hold on %reference
plot(y.time,y.signals.values(:,3),'r'), % y p.o.

plot(ypi.time,ypi.signals.values(:,2),'g'),

plot(yi.time,yi.signals.values(:,3),'m'),
plot(yaw.time,yaw.signals.values(:,3),'k')
legend('r','y','ypi','yi','yaw')
grid

subplot(212)
stairs(y.time,y.signals.values(:,1)), hold on %reference
plot(u.time,u.signals.values(:,2),'r'), % y p.o.

plot(upi.time,upi.signals.values(:,2),'g'),

plot(ui.time,ui.signals.values(:,2),'m'),
plot(uaw.time,uaw.signals.values(:,2),'k')
legend('r','u','upi','ui','uaw')
grid


figure(2); clf,
subplot(211)
stairs(y.time,y.signals.values(:,1)), hold on %reference

plot(y.time,y.signals.values(:,3),'r'), % y p.o.
%plot(ypi.time,ypi.signals.values(:,3),'g'),
plot(yaw.time,yaw.signals.values(:,3),'k')
%legend('r','y','ypi','yaw')
legend('r','y','yaw')
grid

subplot(212)
stairs(y.time,y.signals.values(:,1)), hold on %reference

plot(u.time,u.signals.values(:,2),'r'), % y p.o.
% plot(upi.time,upi.signals.values(:,2),'g'),
plot(uaw.time,uaw.signals.values(:,2),'k')
legend('r','u','uaw')
grid