% This script communicates with the program TSL2561_data_acquisition running on
% an arduino supported atmega uc. It instructs the uc how the TSL2561's
% sensitivity and integration time should be set and receives the counts output
% from the uc.
%
% The atmega uc is controlled with simple commands over the serial line in the
% format cnnn, where 'c' means command and 'nnn' is a 3 character argument.
% s - stands for sensitivity, valid arguments are 1 and 16.
% i - stands for integration time, valid arguments are 13, 101, and 402.
% t - stands for transmit, valid arguments are 001-999. the transmit command
%       instructs the uc to enable the TSL2561 and output nnn measurements taken
%       by the frequency meter.
% l - stands for light, valid arguments are 000 or 111. if the argument is
%       000/111 then the pin specified in the uc code will be brought low/high.
%       this pin can be used to drive an LED or switch a transitor or relay 
%       off/on to control power to a lamp.
%
% The output counts is recorded when the lamp is on (counts_light) and off 
% (counts_dark) so that the photodiode's thermal current, measured by
% counts_dark, can be removed.
%
% counts_light and counts_dark are not automatically saved. You should check for
% bad data before saving. If bad samples are present, remove them and run
% the script save_data.
%
% Tested in:
% -- WinXP: MATLAB 7.5.0 (R2700b), Octave 3.2.4
% -- Debian Wheezy: Octave 3.2.4
%
% MATLAB NOTES:
% fgetl doesn't return -1 when it reaches the end of the stream. Therefore, the
% uc sends a special end of transmission 'EOT' signal.
%

%**** USER SUPPLIED DATA ****%

if strcmpi(filesep, '/') % in Linux
	port = '/dev/ttyUSB0';
	% same tty magic phrase used by arduino
	stty_arg = ['10:0:8bd:0:3:1c:7f:15:4:0:0:0:11:13:1a:0:12:f:17:16:0:0:' ...
	            '0:0:0:0:0:0:0:0:0:0:0:0:0:0'];
	system(['/bin/stty --file=' port ' ' stty_arg]);
elseif strcmpi(filesep, '\') % in Windows
	port = 'COM9';
end

sensitivity = 16;
int_time = 101;
num_samples = 20; % number of frequency measurements.

light_pos = 8;
slit_pos = 2.5;
% distance of light source from detector, [m]
distance = (light_pos-slit_pos)*0.0254;
lamp_type = 'CFL';

% this is used by the script save_data.
fname = ['s' num2str(sensitivity) 'i' num2str(int_time) '_' lamp_type '_' ...
         num2str(light_pos) '.mat'];

%**** END USER SUPPLIED DATA ****%

counts_light = -1;
counts_dark = -1;

if (exist('OCTAVE_VERSION'))
	serial_write = @(spr, sstr) fputs(spr, sstr);
else
	serial_write = @(spr, sstr) fprintf(spr, '%s', sstr);
end

% pause_time is the amount of time to allow the serial buffer to fill with
% samples. one sample takes about 1.2 seconds to generate. 0.1 seconds per
% sample is added to be safe.
pause_time = (1.2 + 0.1)*(num_samples+2); % seconds
%pause_time = 20; %seconds, *DEBUG*DEBUG*DEBUG*DEBUG*


sp = serial_open(port);

serial_write(sp, 'l111'); %turn on lamp (optional).

% by not having a newline the cursor stays at the end of the line.
fprintf('Please turn on lamp. Then press the Enter key to continue.')
% the pause command without an argument did not halt the script, so using
% input instead.
ign = input('', 's');
fprintf('\nThank you. Proceeding.')

scmd = sprintf('s%03d', sensitivity);
serial_write(sp, scmd); %set sensitivity
icmd = sprintf('i%03d', int_time);
serial_write(sp, icmd); %set integration time
tcmd = sprintf('t%03d', num_samples+2); %+2 because first two are discarded.
serial_write(sp, tcmd); %tell uc to start transmitting.

pause(pause_time); %wait for data to be generated and transmitted to buffer

k = 1;
ign = fgetl(sp); % the first sample is usually bad, so discard it.
ign = fgetl(sp); % the second sample is usually bad, so discard it.
c = fgetl(sp);
while (ischar(c) && ~strcmp(c, 'EOT') && k <= num_samples)
	counts_light(k,:) = str2double(c);
	k = k+1;
	c = fgetl(sp);
end

%serial_write(sp, 's000'); %tell TSL230R to power down
% once fgetl has returned -1 (reached EOF) the serial port must be
% closed and re-opened to get any new data.
fclose(sp);

fprintf('\nFinished gathering counts with lamp on.\n\n');

%------------------------------------------------------------------------------%
%------------------------------------------------------------------------------%

%re-open serial port
sp = serial_open(port);

serial_write(sp, 'l000'); %turn off lamp (optional).

fprintf('Please turn off lamp. Then press the Enter key to continue.')
%pause
% the pause command without an argument did not halt the script, so using
% input instead.
ign = input('', 's');
fprintf('\nThank you. Proceeding.')

% sometimes re-opening serial port resets uc so the sensitivity and frequency
% scaling commands must be resent.
scmd = sprintf('s%03d', sensitivity);
serial_write(sp, scmd); %set sensitivity
icmd = sprintf('i%03d', int_time);
serial_write(sp, icmd); %set integration time
tcmd = sprintf('t%03d', num_samples+2); %+2 because first two are discarded.
serial_write(sp, tcmd); %tell uc to start transmitting.

pause(pause_time); %wait for data to be generated and transmitted to buffer

k = 1;
ign = fgetl(sp); % the first sample is usually bad, so discard it.
ign = fgetl(sp); % the second sample is usually bad, so discard it.
c = fgetl(sp);
while (ischar(c) && ~strcmp(c, 'EOT') && k <= num_samples)
	counts_dark(k,:) = str2double(c);
	k = k+1;
	c = fgetl(sp);
end

%serial_write(sp, 's000'); %tell TSL230R to power down
fclose(sp);

fprintf('\nFinished gathering counts with lamp off.\n\n');

%------------------------------------------------------------------------------%
%------------------------------------------------------------------------------%

counts_light
counts_dark

disp('Check for bad data before saving.');
rsp = input('Do you want to save? [(y)es/(n)o]: ', 's');
if (~isempty(rsp) && lower(rsp(1)) == 'y')
	save_data;
end

