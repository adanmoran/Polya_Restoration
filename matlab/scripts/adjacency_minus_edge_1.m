%% Remove edges from adjacency
% Inputs:
% Adjacency - a sparse matrix with adjacencies of radius 1 in the 1-norm
% Edge - a sparse matrix with 1's where there are edges in the original
%        image
%
% Returns a new sparse adjacency matrix
function adj = adjacency_minus_edge_1(adjacency, edges)

    % Compute the indices of the ones in a sparse matrix
    [ii, jj] = find(edges);
    
    % Determine which pixel index in the adjacency matrix must be removed
    values = ones(size(ii));
    pixel_numbers = pixel_number(ii, jj, size(edges,2));
    
    edge_identity = sparse(                 ...
                        pixel_numbers,      ...
                        pixel_numbers,      ...
                        values,             ...
                        size(adjacency,1),  ...
                        size(adjacency,2));

    % Create a sparse identity matrix which has zeros wherever there are
    % edges
    quasi_identity = speye(size(adjacency)) - edge_identity;
    
    % We can use adjacency instead of adjacency' because it's symmetric
    adj = (quasi_identity * adjacency * quasi_identity) + edge_identity;
end