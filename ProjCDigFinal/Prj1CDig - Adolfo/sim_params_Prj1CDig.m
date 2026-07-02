%Kp = 0.8864 +/- 0.0015003           
%Tp1 = 323.16 +/- 4.396               
%Tp2 = 37.636 +/- 0.071432            
%Td = 5                              
%Tz = 278.32 +/- 3.8386

Ta = 23 % [ºC]
PO = 60 % [ºC]
w0 = 0.098


P = -0.10190952513095525 %  polo do compensador PI, no Control System Design
I = 3.2801               % ganho do compensador PI, no Control System Design

A = (modelo.Tz - modelo.Tp1)/(modelo.Tp2 - modelo.Tp1)
B = 1 - A
