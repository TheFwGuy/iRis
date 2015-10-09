% Normally this script will be called from get_data, but if the data has
% bad samples you should tell get_data not to save, remove the bad
% samples then call this script to save.

s = filesep;

save('-V6', ['data' s 'TSL2561_meter_output' s fname], 'sensitivity', ...
     'int_time', 'distance', 'counts_light', 'counts_dark')
