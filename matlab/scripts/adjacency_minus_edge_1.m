%% Remove edges from adjacency
% Inputs:
% Adjacency - a sparse matrix with adjacencies of radius 1 in the 1-norm
% Edge - a sparse matrix with 1's where there are edges in the original
%        image
%
% Returns a new sparse adjacency matrix
function adj = adjacency_minus_edge_1(adjacency, edges)

    adj = adjacency;

    % Compute the indices of the ones in a sparse matrix
    [ii, jj] = find(edges);
    
    % Determine which pixel index in the adjacency matrix must be removed
    values = ones(size(ii));
    pixel_numbers = (ii - values) .* size(edges,2) + jj;
    
    edge_identity = sparse(                 ...
                        pixel_numbers,      ...
                        pixel_numbers,      ...
                        values,             ...
                        size(adjacency,1),  ...
                        size(adjacency,2));

    % Create a sparse identity matrix which has zeros wherever there are
    % edges
    quasi_identity = speye(size(adjacency)) - edge_identity;
%     I = speye(size(adjacency));
%     
%     for k = 1:length(ii)
%         % pixel (i,j) of an nxn image has adjacency row (i-1)*n + j
%         pixel_number = (ii(k)-1) * size(edges,2) + jj(k);
%         
%         I(pixel_number, pixel_number) = 0;
% 
% %         % Remove the edge from its neighbours' super urns
% %         adj(:, pixel_number) = zeros(length(adj), 1);
% % 
% %         % cut out the connections to the edge, but keep a connection to itself
% %         adj(pixel_number, :) = zeros(1, length(adj));
% %         adj(pixel_number, pixel_number) = 1;
%     end
    
    adj = (quasi_identity * adjacency' * quasi_identity) + edge_identity;
end