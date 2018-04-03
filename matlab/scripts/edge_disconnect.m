%% Edge Disconnect
% Creates a sparse matrix representing locations in an adjacency matrix
% where the adjacencies should be removed, for a distance of 2.
%
% Inputs:
% ii = "rows" where an edge can be found
% jj = "columns" where an edge can be found
% min = minimum row value after which you allow edge disconnection 
%   (i.e. disconnect at rows > min)
% max = maximum row value before which you allow edge disconnects
%   (i.e. disconnect at rows < max)
% n = size of the nxn adjacency matrix
% m = width of the image
% direction = direction of disconnection around edges. Values are
% 'vertical' or 'horizontal' (default: 'vertical')
%
% Outputs:
% S = a sparse matrix representing locations where adjacencies should be
% removed
function S = edge_disconnect(ii, jj, min, max, n, m, direction)
    switch(nargin)
        case 6
            direction = 'vertical';
        case 7
            if ~strcmp(direction,'vertical') ...
                && ~strcmp(direction,'horizontal')
                error('Can only remove edges vertically or horizontally.');
            end
    end
    % Locations of pixels which can be vertically filtered
    slice = ii > min & ii < max;
    % Get the "columns" of the pixels which can be vertically filtered
    vertical_j = jj(slice);
    % "rows" of pixels which can be vertically filtered
    vertical_i = ii(slice);
    
    % Pixels above the edge
    one = ones(size(vertical_i));
    one_up = vertical_i - one;
    % Pixels below the edge
    one_down = vertical_i + one;
    
    if strcmp(direction,'horizontal')
        % Get the adjacency row locations for pixels to the left of the
        % edge
        pixels_up = pixel_number(vertical_j, one_up, m);
        pixels_down = pixel_number(vertical_j, one_down, m);
    else
        % Get the adjacency row locations for the pixels above the edge
        pixels_up = pixel_number(one_up, vertical_j, m);
        pixels_down = pixel_number(one_down, vertical_j, m);
    end
    
    % Pixels that are one above the edge are disconnected from one below
    S = sparse(pixels_up, pixels_down, ones(size(pixels_up)), n, n);
    % Must add transpose to fully disconnect these pixels
    S = S + S';
end