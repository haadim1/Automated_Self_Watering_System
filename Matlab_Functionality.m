clear all; close all;
a = arduino ('COM5', 'UNO')

stop = false;
while ~stop 
    x = readVoltage(a, 'A1')
    if (x>=3.4)
        %beep
        writePWMDutyCycle(a, 'D5', 0.33); 
        pause(0.5); 
        writeDigitalPin(a,'D5',0)
        %Flash
        writeDigitalPin(a,'D4',1); 
        pause(2); 
        writeDigitalPin(a,'D4',0) 
        %pump
        writeDigitalPin(a,'D2',1);
        pause(5)
    else 
         writeDigitalPin(a,'D2', 0);
         pause(2.5)
    end
    stop = readDigitalPin(a, 'D6');
end

writeDigitalPin(a, 'D2',0);

