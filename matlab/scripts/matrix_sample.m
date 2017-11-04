%% Matrix Sample
% Sample from an nxk weighting matrix. The weightings will be normalized.
% 
% Inputs:
% W - weighting matrix
%
% Returns:
% index - an nxk logical matrix where each row is 0 
% everywhere except for one position, which has a value of 1. This index
% location specifies which column was pixed from the original weighting
% matrix for that given row.
%
% e.g.
%      Black    Red
% W = [ 0.3     0.7
%       0.5     0.5]
% 
% index: 
%     [  0       1
%        1       0 ]
% indicates that the first row picked a "Black" ball and the second row
% picked a "Red" ball.
%
% Source: 

function index = matrix_sample(W)
    % Flip weighting matrix and normalize it column-wise
    W_flipped = W';
    W_normalized = W_flipped./repmat(sum(W_flipped,1),size(W_flipped,1),1);
    % Take the cumulative sum of the rows of flipped weighting matrix
    W_cdf = cumsum(W_normalized, 1);
    % Randomly sample along those rows
    x = rand(1, size(W_normalized,2));
    % Repeat random sampling to get a matrix of the correct size
    C = repmat(x, size(W_normalized,1),1)<W_cdf;
    
    % Compute the flipped index matrix by x-oring the columns
    index = [C(1,:); xor(C(2:end,:),C(1:end-1,:))];
    % Get the actual index matrix in nxk format.
    index = index';

end