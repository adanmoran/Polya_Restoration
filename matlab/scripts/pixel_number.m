%% Pixel Number
% Get the row for the adjacency matrix based on the pixel position in an
% image.
% 
% Inputs:
% ii = row values
% jj = column values
% m = number of columns in the image
%
% Output:
% pixels = pixel numbers, calculated horizontally from top to bottom.

function pixels = pixel_number(ii, jj, m)
    pixels = (ii - ones(size(ii))) .* m + jj;
end