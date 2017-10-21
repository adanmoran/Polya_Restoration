%% Get a sparse representation of an adjacency matrix for an n x m graph
% Wrapper for sparse_adj_matrix function to output a sparse adjacency
% matrix directly.
%
% Inputs:
%   sz - grid size ([numRows, numCols])
%   radius  - the radius around each point for which edges are formed
%   p_norm  - in what p-norm to measure the r-ball, can be 1,2 or 'inf'
%
% Ex. 1-norm, r = 1 vs. inf-norm, r=1
%    o                     o  o  o
% o  X  o           vs.    o  X  o
%    o                     o  o  o
%
% Outputs:
%   adj - a sparse matrix of the adjacency representation
%       - can open up the sparse matrix using full(adj)
function adj = get_sparse_adj(rc, radius, p_norm)

[ii, jj] = sparse_adj_matrix(rc, radius, p_norm);

adj = sparse(ii, jj, ones(1,numel(ii)), prod(rc), prod(rc));
end