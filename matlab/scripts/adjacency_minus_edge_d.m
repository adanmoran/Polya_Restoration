%% Adjacency Minus Edge For a Distance d
% Computes the adjacency matrix without edge connections for a distance d
% in an Lp norm. This function only works for a distance d = 2.
%
% Inputs:
%   - adjacency: a sparse matrix with adjacencies of radius d in an Lp norm
%   - edges: a sparse matrix with 1's where there are edges in the original
%        image
%   - d: the neighbourhood distance used to generate the adjacency matrix
%
% Output:
%   - adj: a sparse matrix of adjacencies without the edge connections
%       provided by edges input
function adj = adjacency_minus_edge_d(adjacency, edges, d)
    adj = adjacency;
    % Remove edges for d = 2
    if d == 2
        % Size values for edge disconnection
        N = size(adjacency, 1);
        [numRows, numCols] = size(edges);

        % Get the list of edge locations
        [ii, jj] = find(edges);

        % Disconnect edges "vertically"
        adj = adj - edge_disconnect(ii, jj, 1, numRows, N, numCols, 'vertical');

        % Disconnect edges "horizontally"
        adj = adj - edge_disconnect(jj, ii, 1, numCols, N, numCols, 'horizontal');
    end
    % Remove the edges from all adjacent pixels
    adj = adjacency_minus_edge_1(adj, edges);
end