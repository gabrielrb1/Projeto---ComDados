close all
clear
clc
%Geração do sinal RS485

tm = 1e-9; %nano segundos
n = 8;  %tamanho do sinal (bits)
l = 1000; %Tamanho do bit (ps)
A = 4;      %amplitude
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
Qb = -Qa;
Ra = Mr(:);
Qr = Qa-Qb;

% stairs(Ra)
% ylim([-1.1*A 1.1*A])
% title('Sinal RS485 aleatório')
% xlabel('Tempo (ps)')
% ylabel('Amplitude (V)')

figure
stairs(Qa)
hold on
stairs(Qb)
stairs(Qr)
%ylim([-1.1*A 1.1*A])
title('Sinal RS485 periódico')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')

sinalf = Qr;
%%
%Fourrier
Fs = 1/(tm*l);            % Sampling frequency    Fs(real) = 500Hz, 1MHz para amostragem, porém deve ser dividido por l pois o vetor esta em ps               
T = 1/Fs;             % Sampling period       
L = n*l;             % Length of signal

Y = fft(Qa);
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
hb = 5e7;           
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

 Ssa = zeros(1,n*l)';

for k=1:length(f)
    if f(k) >= lb && f(k) <= hb
        Ssa = Ssa + P1(k)*sin(2*pi*f(k)*ts);    
    end
end

Ssb = -Ssa;


figure
plot(Ssa(1500:3500))
title('Sinal do canal A reconstruido')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')

%cálculo do erro

E = Ssa - sinalf;
Eabs = abs(E);
Em = mean(Eabs);
Ermse = sqrt(mean((E).^2));

figure
plot(E)
title('Erro')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')

figure
plot(Ssa(1500:3500))
hold on
plot(Ssb(1500:3500))
plot(Ssa(1500:3500)-Ssb(1500:3500))
title('Sinal RS485 reconstruido')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')


%%
%Interferência
tse = (0:1000*tm:3000*(n*(tm*l))-(tm))';
Sh = zeros(1,size(tse,1))';
Sht = zeros(1,size(tse,1))';

SsM = Ssa;

figure
subplot(2,1,1)
for h=1:7
Sh(:,h)= (1/(3^(h-1)))*sin(2*pi*60*h*tse);
Sht = Sht + Sh(:,h);
plot(Sh(:,h))
hold on
SsM = SsM + (2/(3^(h-1)))*sin((2*pi*60*h*ts)+deg2rad(90));
end
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')
title('Componentes')
legend('60Hz','120Hz','180Hz','240Hz','300Hz','360Hz','420Hz','location', 'northeastoutside')


subplot(2,1,2)
plot(Sht)
title('Somatório Harmonicos')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')
legend('Resultante','location', 'northeastoutside')

figure
plot(SsM)
title('Efeito da interferência eletromagnética')
xlabel('Tempo (ps)')
ylabel('Amplitude (V)')
hold on
plot(Ssa)
legend('Sinal com interferência','Sinal RS485 original')

%%
%eco
sinala = Ssa;
sinalb = Ssb;
d=45;
SNR = 1.1;                         
delta = round(l + l*(d/360));            %nano segundos

origa = [sinala;zeros(delta,1)];
echoa = [zeros(delta,1);sinala]*(1/SNR);

origb = [sinalb;zeros(delta,1)];
echob = [zeros(delta,1);sinalb]*(1/SNR);

mtEchoa = origa + echoa;
mtEchob = origb + echob;


t = (0:length(mtEchoa)-1)/l;

figure
subplot(2,1,1)
plot(t(1:8000),[origa(1:8000) echoa(1:8000)])
legend('Original','Eco')
title('Canal A')

subplot(2,1,2)
plot(t(1:8000),mtEchoa(1:8000))
legend('Total')
xlabel('Tempo (ns)')

figure
subplot(2,1,1)
plot(t(1:8000),[origb(1:8000) echob(1:8000)])
legend('Original','Eco')
title('Canal B')

subplot(2,1,2)
plot(t(1:8000),mtEchob(1:8000))
legend('Total')
xlabel('Tempo (ns)')

figure
plot(t(1:8000),mtEchoa(1:8000)-mtEchob(1:8000))
title('Sinal RS485')
xlabel('Tempo (ns)')
%%
%interferencia construtiva sem gibbs

Se = 0:0.01:6;
Ab = 1;
Sr = Se.*Ab;
SNR = 1./((6./Sr)-1);
figure
plot(Se,SNR)
title('Relação SNR e Amplitude do sinal emitido para diferentes níveis de atenuação no trajeto emissor-receptor') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')
grid on
hold on
xlim([0 6])
set(gca,'Yscale','log')

Ab = 0.75;
Sr = Se.*Ab;
SNR = 1./((6./Sr)-1);
plot(Se,SNR)

Ab = 0.5;
Sr = Se.*Ab;
SNR = 1./((6./Sr)-1);
plot(Se,SNR)

Ab = 0.25;
Sr = Se.*Ab;
SNR = 1./((6./Sr)-1);
plot(Se,SNR)
hold on

legend('Atenuação de 0.00','Atenuação de 0.25','Atenuação de 0.50','Atenuação de 0.75','location', 'northwest')
%%
%interferencia construtiva com gibbs

Se = 0:0.01:6;
Sg = Se.*1.18;
Ab = 1;
Sr = Sg.*Ab;
SNR = 1./((5.08./Sr)-1);
figure
plot(Se,SNR)
title('Relação SNR e Amplitude do sinal emitido para diferentes níveis de atenuação no trajeto emissor-receptor') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')
grid on
hold on
xlim([0 6])
set(gca,'Yscale','log')

Ab = 0.75;
Sr = Sg.*Ab;
SNR = 1./((5.08./Sr)-1);
plot(Se,SNR)

Ab = 0.5;
Sr = Sg.*Ab;
SNR = 1./((5.08./Sr)-1);
plot(Se,SNR)

Ab = 0.25;
Sr = Sg.*Ab;
SNR = 1./((5.08./Sr)-1);
plot(Se,SNR)
hold on

legend('Atenuação de 0.00','Atenuação de 0.25','Atenuação de 0.50','Atenuação de 0.75','location', 'northwest')
%%
%interferencia destrutiva sem leitura

Se = 0:0.01:6;
Ab = 1;
Sr = Se.*Ab;
SNR = -1./((0.2./Sr)-1);
figure
plot(Se,SNR)
title('Relação SNR e Amplitude do sinal emitido para diferentes níveis de atenuação no trajeto emissor-receptor') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')
grid on
hold on
xlim([0 6])
set(gca,'Yscale','log')

Ab = 0.75;
Sr = Se.*Ab;
SNR = -1./((0.2./Sr)-1);
plot(Se,SNR)

Ab = 0.5;
Sr = Se.*Ab;
SNR = -1./((0.2./Sr)-1);
plot(Se,SNR)

Ab = 0.25;
Sr = Se.*Ab;
SNR = -1./((0.2./Sr)-1);
plot(Se,SNR)
hold on

legend('Atenuação de 0.00','Atenuação de 0.25','Atenuação de 0.50','Atenuação de 0.75')

%%
%interferencia destrutiva com leitura errada

Se = 0:0.01:6;
Ab = 1;
Sr = Se.*Ab;
SNR = -1./((-0.2./Sr)-1);
figure
plot(Se,SNR)
title('Relação SNR e Amplitude do sinal emitido para diferentes níveis de atenuação no trajeto emissor-receptor') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')
grid on
hold on
xlim([0 6])
set(gca,'Yscale','log')

Ab = 0.75;
Sr = Se.*Ab;
SNR = -1./((-0.2./Sr)-1);
plot(Se,SNR)

Ab = 0.5;
Sr = Se.*Ab;
SNR = -1./((-0.2./Sr)-1);
plot(Se,SNR)

Ab = 0.25;
Sr = Se.*Ab;
SNR = -1./((-0.2./Sr)-1);
plot(Se,SNR)
hold on

legend('Atenuação de 0.00','Atenuação de 0.25','Atenuação de 0.50','Atenuação de 0.75')
%%
%Entre curvas

Se = 0:0.01:6;
Sg = Se.*1.18;
Ab = 1;
Sr = Sg.*Ab;
SNRu = 1./((5.08./Sr)-1);

Se = 0:0.01:6;
Ab = 1;
Sr = Se.*Ab;
SNRd = -1./((0.2./Sr)-1);

SNR = [SNRd(1:225) SNRu(226:end)];
figure
plot(Se,SNR)
hold on
grid on
set(gca,'Yscale','log')
title('Relação SNR e Amplitude do sinal emitido') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')

Ab = 0.75;
Sr = Sg.*Ab;
Sr = Se.*Ab;
SNRu = 1./((5.08./Sr)-1);
SNRd = -1./((0.2./Sr)-1);
SNR = [SNRd(1:352) SNRu(353:end)];
plot(Se,SNR)

legend('Atenuação de 0.00','Atenuação de 0.25')

%%
%Interferência eletromagnética construtiva
figure

Se = 0:0.01:6;
Sg = Se.*1.18;
Ab = 1;
SNRau = ((Ab)./((6./Sg)-1));
plot(Se,SNRau)
hold on
Ab = 0.75;
SNRbu = ((Ab)./((6./Sg)-1));
plot(Se,SNRbu)
Ab = 0.5;
SNRcu = ((Ab)./((6./Sg)-1));
plot(Se,SNRcu)
Ab = 0.25;
SNRdu = ((Ab)./((6./Sg)-1));
plot(Se,SNRdu)

title('Relação SNR e Amplitude do sinal emitido') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')
set(gca,'Yscale','log')
legend('Atenuação  de 0.00','Atenuação de 0.25','Atenuação de 0.50','Atenuação de 0.75')
grid on
hold off
%%
%Interferência eletromagnética destrutiva
figure

Se = 0:0.01:6;
Sg = Se.*1.18;
Ab = 1;
SNRad = -((Ab)./(((0.2)./Sg)-1));
plot(Se,SNRad)
hold on
Ab = 0.75;
SNRbd = -((Ab)./(((0.2)./Sg)-1));
plot(Se,SNRbd)
Ab = 0.5;
SNRcd = -((Ab)./(((0.2)./Sg)-1));
plot(Se,SNRcd)
Ab = 0.25;
SNRdd = -((Ab)./(((0.2)./Sg)-1));
plot(Se,SNRdd)

title('Relação SNR e Amplitude do sinal emitido') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')
set(gca,'Yscale','log')
legend('Atenuação  de 0.00','Atenuação de 0.25','Atenuação de 0.50','Atenuação de 0.75')
grid on
hold off

%%
SNRa = [SNRad(1:263) SNRau(264:end)]; 
SNRb = [SNRbd(1:263) SNRbu(264:end)]; 
SNRc = [SNRcd(1:263) SNRcu(264:end)]; 
SNRd = [SNRdd(1:263) SNRdu(264:end)]; 

figure
plot(Se,SNRa)
hold on
plot(Se,SNRb)
plot(Se,SNRc)
plot(Se,SNRd)
title('Relação SNR e Amplitude do sinal emitido') 
xlabel('Amplitude do sinal emitido')
ylabel('SNR mínimo')
set(gca,'Yscale','log')
legend('Atenuação  de 0.00','Atenuação de 0.25','Atenuação de 0.50','Atenuação de 0.75')
grid on
hold off