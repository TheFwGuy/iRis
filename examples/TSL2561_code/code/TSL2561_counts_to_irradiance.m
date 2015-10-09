%TSL2561_counts_to_irradiance - Converts the frequency output of the TSL2561 to
%                               irradiance when given the light source's spectrum.
%
% Syntax:  [Ee, E] = TSL2561_counts_to_irradiance(lambda, X, counts)
%
% Inputs:
%    lambda - the spectrum's wavelength scale.
%         X - the light sources spectrum.
%    counts - the output of the TSL2561.
%
% Outputs:
%    Ee - the light source's spectral irradiance.
%     E - the light source's irradiance.
%
% Example:
%    load('TSL2561_actinic.mat')
%    counts = mean(counts_light) - mean(counts_dark);
%    mu = 415;
%    FWHM = 30;
%    Xactinic = 75*exp(-2.7726*((so.lambda-mu)/FWHM).^2); % model spectrogram
%    Ee_meter = TSL2561_counts_to_irradiance(so.lambda, Xactinic, counts);
%
%    load('TSL2561_actinic.mat')
%    counts = mean(counts_light) - mean(counts_dark);
%    load('actinic_spectrograph.mat'); % loads spctgrph
%    Xactinic = image2spectrum(spctgrph); % spectrogram created from a picture
%    Ee_meter = TSL2561_counts_to_irradiance(so.lambda, Xactinic, counts);
%
%
% Other m-files required: none
% Subfunctions: none
% MAT-files required: none
%
% See also: NONE
%
% Author: Jonathan Thomson
% Work:
% email:
% Website: http://jethomson.wordpress.com
%

%NOTES:
% This m-file uses the counts output from the TSL2561, it's spectral
% responsivity, and a mathematical model of a lamp's spectrum or a
% spectrogram to determine the lamp's irradiance and spectral irradiance.
%
% Assuming the lamp's spotlight fills an area greater than the photodiode's
% aperture then the radiant spectral power (or power spectral density) entering
% the photodiode is the light's spectral irradiance [uW/(cm^2)/nm] multiplied
% by the area of the aperture [cm^2].
% [cm^2]*[uW/(cm^2)/nm] = [uW/nm]
%
% Once a photon has entered the photodiode it has a wavelength dependent
% probability of creating a electron/hole pair that produce a differential
% current. This conversion from spectral radiant power to current is
% represented by the photodiode's spectral responsivity [A/uW]. All of the
% differential currents naturally integrate to form the photodiode's total
% photocurrent because they enter the same conductor.
% integral{[A/uW]*[uW/nm]*[nm]} = [A]
%
% Finally the current-to-counts converter transforms the current to the
% output counts.
% [counts/A]*[A] = [counts]
%
% The total transformation from spectral irradiance to frequency is represented
% by: [counts] = [counts/A]*integral{[A/uW]*[cm^2]*[uW/(cm^2)/nm]*[nm]}
%
% The datasheet only gives the normalized spectral responsivity (as a plot) and
% the system's responsivity at 640 nm, Re(640 nm) = 27.5 [counts/(uW/(cm^2))]
% (when the integration time is 101 ms and sensitivity is 16x). The mat file 
% Re2561.mat is the spectral responsivity of the TSL2561. It was created by 
% converting the datasheet's graph of the normalized spectral responsivity into 
% a point-series and multiplying it by the system's responsivity at 640 nm which 
% is 27.5 [counts/(uW/(cm^2))].
%
% If the spectral irradiance incident on the TSL2561 photodiode is represented
% by A(lambda) then counts = integral{Re(lambda)*A(lambda) dlambda} from 0 to inf.
% However, A(lambda) is unknown so we must use X(lambda) which has the same
% shape as A(lambda) but is off by an unknown multiplicative constant k
% because it is an uncalibrated spectrum (i.e. A(lambda) = k*X(lambda)).
% We can then use our mathematical model of the TSL2561 to find the counts
% output that would result if a light source with a spectrum X(lambda) incident
% on the sensor: cX = integral{Re(lambda)*X(lambda) dlambda}
%
% Since counts is known, cX can be found, and the TSL2561 is a linear system, 
% k can be found simply: k = counts/cX
% Therefore we have obtained the desired result Ee = k*X.
% The accuracy of your result depends on the accuracy of the TSL2561 as well
% as how well X(lambda) matches the shape of A(lambda).
%
% Extra Info
% Operating Characteristics, High Gain (16x), VDD = 3 V, TA = 25C, Tint = 101 ms
% Re = 27.5 [counts/(uW/cm^2)]
% lambda must be in nm and between 250 nm and 1200 nm inclusive.
%


function [Ee, E] = TSL2561_counts_to_irradiance(lambda, X, counts)

	s = filesep;
	% load lambda [nm] and
	% system responsivity
	load(['data' s 'essential_data_sets' s 'Re2561.mat'])

	% interpolate so we can use lambda [nm] instead of lambda_Re
	Rei = interp1(lambda_Re, Re2561, lambda, 'spline', 0);

	% cX is the frequency that would be output if the model lamp shined
	% light with a spectral irradiance X on the photodiode.
	dl = mean(diff(lambda)); % [nm]
	cX = trapz(Rei.*X).*dl; % [counts]

	k = counts./cX; % scaling factor, [dimensionless]

	Ee = k.*X; % spectral irradiance of lamp, [uW/(cm^2)/nm]
	E = trapz(Ee).*dl; % irradiance of lamp, [uW/(cm^2)]

end

