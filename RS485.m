close all
clear
clc
%Geração do sinal RS485

tm = 1e-9; %nano segundos
n = 8;  %tamanho do sinal (bits)
l = 1000; %Tamanho do bit (ps)
A = 6;      %amplitude
R = rand(1,n);

for i=1:n
    Qa(i) = A*(2*mod(i,2)-1);
    
    if R(i) > 0.5
        Ra(i) = A;
    else
        Ra(i) = -A;
    end
end

Mq = zeros(l,n);
Mr = zeros(l,n);

for i=1:n
    Mq(:,i) = Qa(i);
    Mr(:,i) = Ra(i);
end

Qa = Mq(:);
Ra = Mr(:);

stairs(Ra)
ylim([-1.1*A 1.1*A])
title('Sinal RS485 aleatório')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')

figure
stairs(Qa)
ylim([-1.1*A 1.1*A])
title('Sinal RS485 periódico')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')

sinalf = Qa;
%%
%Fourrier
Fs = 1e6;            % Sampling frequency    Fs(real) = 500Hz, 1MHz para amostragem, porém deve ser dividido por l pois o vetor esta em ps               
T = 1/Fs;             % Sampling period       
L = n*l;             % Length of signal

Y = fft(sinalf);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = Fs*(0:(L/2))/L;
f = f*1000; %1000 amostras/bit
figure
stem(f,P1,'filled','.')
title('Frequências')
xlabel('f (Hz)')
ylabel('Amplitude (V)')

%%
%Reconstrução do sinal em meio físico real
ts = (0:tm:(n*(tm*l))-(tm))';

%banda limite
hb = 7.5e6;           
lb = 0;

for i=1:length(f)
    if f(i) >= hb
        hbi = i;
        break
    end
end

for j=1:length(f)
    if f(j) <= lb
        hbj = j;
        break
    end
end

%Somatório de senóides

 Ss = zeros(1,n*l)';

for k=1:length(f)
    if f(k) >= lb && f(k) <= hb
        Ss = Ss + P1(k)*sin(2*pi*f(k)*ts);    
    end
end

figure
plot(Ss)
title('Sinal reconstruido')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')

%cálculo do erro

E = Ss - sinalf;
Eabs = abs(E);
Em = mean(Eabs);
Ermse = sqrt(mean((E).^2));

figure
plot(E)
title('Erro')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')

%%
%Interferência

Sh = zeros(1,n*l)';

for h=1:7
Sh = Sh + (1/(3^(h-1)))*sin(2*pi*60*h*ts);
end
figure
plot(Sh)
figure
plot(Ss+Sh)

%%
%eco
sinal = Ss;
delta = round(1150);            %nano segundos
atenuacao = 0.4;                %atenuação do reflexo

orig = [sinal;zeros(delta,1)];
echo = [zeros(delta,1);sinal]*(1-atenuacao);

mtEcho = orig + echo;

t = (0:length(mtEcho)-1)/l;

figure
subplot(2,1,1)
plot(t,[orig echo])
legend('Original','Eco')

subplot(2,1,2)
plot(t,mtEcho)
legend('Total')
xlabel('Tempo (ns)')


