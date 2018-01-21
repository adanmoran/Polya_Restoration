%% Polya Contagion Model
% This model 
% V = Urn Matrix; A nxk matrix where n is the number of vertices, and k is
% the number of different types of balls
%
% A = Adjacency Matrix; a binary nxn matrix where a 1 represents 
% a connection between the vertices (i,j). Note that A must contain
% 1's along the diagonal so that the superurn can be correctly retrieved.
%
% Delta = a kxk matrix where the (i,j)th entry denotes the number of balls 
% of type j that will be added when a ball of type i is drawn.
%
% SampleType (optional) = a string telling the polya model how to sample
% from the urns. Options are 'random' (for random urn draws) or 'median'
% (for drawing the median urn). Default is 'random'.
function newV = polya(V, A, Delta, sampleType)
    switch(nargin)
        case 3
            sampleType = 'random';
        case 4
            if ~strcmp(sampleType,'random') ...
                && ~strcmp(sampleType,'median')
                error('Can only remove edges vertically or horizontally.');
            end
    end
    % How many types of balls we have
    k = size(Delta,1);
    
    % Compute the superurn of each pixel. As a matrix, this is A*V
    S = A * V;
    
    % Vector containing the total number of balls for each superurn row
    T = S * ones(k, 1);
    
    % nxk matrix of weightings for each row of the superurn matrix
    W = S ./ T;
    
    % Sample from the superurn using the weighting matrix to get the balls
    % to add
    if strcmp(sampleType, 'median') 
        % Pick out the median, which is equivalent to the max value.
        % If the urns are zero, we pick black.
        %S_flip = flip(S,2);
        %B = ((S >= S_flip & [1 0]) | (S > S_flip))*Delta;
        
        % Find the median ball location and make a matrix with a one where
        % it is
        C = cumsum(S,2); % Tally up the rows
        % Locate the median value along those rows by finding the first
        % location where the cumulative sum is greater than the total / 2.
        % This max function returns a list of the columns
        [~, medians] = max(C > (T./2), [], 2);
        % Build the matrix out of these cumulative sum indices
        B = sparse(1:length(medians), ...
                            medians, ...
                            ones(size(medians)),...
                            length(medians),...
                            size(Delta,2))*Delta;
    else
        B = matrix_sample(W)*Delta;
    end
    
    % Add the balls to the original graph
    newV = V + B;
end
