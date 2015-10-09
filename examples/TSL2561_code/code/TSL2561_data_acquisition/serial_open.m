%SERIAL_OPEN - Opens the device file supplied as the argument "port" for
%              serial communication in the manner required by GNU Octave
%              or MATLAB.
%
% GNU Octave and MATLAB handle opening a serial port differently. This function
% detects which environment it is in and opens the serial port accordingly.
%
% Syntax:  sp = serial_open(port)
%
% Inputs:
%    port - the device file or communications port
%
% Outputs:
%    sp - the handle for sending and receiving data over the serial port
%
% Example:
%    sp = serial_open('/dev/ttyUSB0'); % UNIX like systems
%    sp = serial_open('C0M9'); % Windows
%
%
% Other m-files required: none
% Subfunctions: none
% MAT-files required: none
%
% See also:
%
% Author: Jonathan Thomson
% Work:
% email:
% Website: http://jethomson.wordpress.com
%
%
% MATLAB terminator notes:
% Additionally, you can set Terminator to a 1-by-2 cell array. The first element
% of the cell is the read terminator and the second element of the cell array is 
% the write terminator.
% When performing a write operation using the fprintf function, all occurrences 
% of \n are replaced with the Terminator value. Note that %s\n is the default
% format for fprintf. A read operation with fgetl, fgets, or fscanf completes
% when the Terminator value is read. The terminator is ignored for binary
% operations.
% Source: http://www.mathworks.com/help/toolbox/instrument/terminator.html



function sp = serial_open(port)

	BOOTLOADER_PAUSE_TIME = 3; % seconds

	fprintf('Opening serial port. ');

	if exist('OCTAVE_VERSION')
		sp = fopen(port, 'r+');
	else
		% Terminator values are line feed ('\n') when receiving data
		% and nothing (no terminator) when sending data.
		sp = serial(port, 'BaudRate', 9600, ...
		            'Terminator', {'LF', ''});
		fopen(sp);
	end
	% opening the serial port in MATLAB causes the uc to reset.
	% pause to allow the bootloader to timeout.
	fprintf('Pausing for bootloader. ');
	pause(BOOTLOADER_PAUSE_TIME);
	disp('Done.')
end
