% From the TSL2561 datasheet:
% "The 640 nm irradiance Ee is supplied by an AlInGaP light-emitting diode with
% the following characteristics: peak wavelength λp = 640 nm and spectral 
% halfwidth ∆λ1⁄2 = 17 nm." --> mu = 640 nm, FWHM = 2*17 nm
%
% With a gain of 16x, Vdd = 3V, Ta = 25°C, λp = 640 nm, Tint = 101 ms, and 
% Ee = 36.3 uW/cm^2 the typical counts measured by the Channel 0 diode is 1000.

s = filesep;
load(['data' s 'essential_data_sets' s 'Re2561.mat']);
dlambda = lambda_Re(2)-lambda_Re(1);

% Re2561.mat : (irradiance responsivity @ 640 nm of Ch0 with int. time of 101 ms = 27.5 [counts/(uW/cm^2)])
%             *(normalized spectral responsivity of Ch0)

e = exp(1);
Fled = @(mu, FWHM) e.^(-2.7726*((lambda_Re-mu)/FWHM).^2);
Xred = Fled(640, 2*17);
k = trapz(Xred).*(dlambda);
Ee = (36.3/k)*Xred; % model of AlInGaP test LED, [uW/nm/cm^2]

% datasheet says typical counts for AlInGaP test LED should be 1000
% counts = 996.08
% pct error = 100*(1000 - 996.08)/1000 = 0.39211 %
counts = trapz(Re2561.*Ee).*(dlambda);
