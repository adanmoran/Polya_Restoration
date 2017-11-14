% Compute the adjacency minus edges for an L1 distance d
function adj = adjacency_minus_edge_d(adjacency, edges, d)
    adj = adjacency;
    % Remove edges for d = 2
    if d == 2
        % Size values for edge disconnection
        N = size(adjacency,1);
        numRows = size(edges,1);
        numCols = size(edges,2);

        % Get the list of edge locations
        [ii, jj] = find(edges);

        % Get disconnections "vertically"
        adj = adj - edge_disconnect(ii, jj, 1, numRows, N, numCols, 'vertical');

        % Get disconnections "horizontally"
        adj = adj - edge_disconnect(jj, ii, 1, numCols, N, numCols, 'horizontal');
    end
    % Remove the edges from all adjacent pixels
    adj = adjacency_minus_edge_1(adj, edges);
end