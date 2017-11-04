%% Adjacency Minus Edges
% This method removes edges from an adjacency matrix for Lena.
% This only works for a radius of 1 using the 1-norm
clear;
close all;
clc;

%% Test with tiny image
adjacency = get_sparse_adj([3 3], 1, 1);
edges = [ 0 1 0;
          0 0 1;
          0 0 0];
 edges = sparse(edges);
 
% Get the indices of the ones in the sparse edge matrix
 [ii, jj] = find(edges);
 
 for k = 1:length(ii)
    % pixel (i,j) of an nxn image has adjacency row (i-1)*n + j
    pixel_number = (ii(k)-1) * size(edges,2) + jj(k);
    
    % Remove the edge from its neighbours' super urns
    adjacency(:, pixel_number) = zeros(length(adjacency), 1);
    
    % cut out the connections to the edge, but keep a connection to itself
    adjacency(pixel_number, :) = zeros(1, length(adjacency));
    adjacency(pixel_number, pixel_number) = 1;
 end

% Iterate over the ones and remove them from other elements
% 
% %% Load
% % test_bw creates: C: the noisified image of Lena edges: the edge matrix
% % for Lena
% test_bw;
% edges = sparse(edges);
% 
% %% Compute Adjacency Matrix
% % Adjacency matrix for lena with a radius of 1 in the 1 norm
% adjacency = get_sparse_adj(size(C), 1, 1);
% 
% %% Remove Edges
% % Iterate through all the edges "1"-values and remove them from the
% % adjacency matrix