%% Add Noise to an Image Based on a Gauss-Markov Process
% http://www.mast.queensu.ca/~fady/TR/TR_94-79.pdf
% Section 4.2
% 
% Based on recursion V_i = correlation * V_i-1 + U_i
% V_0 = 0
% 
% Inputs:
%     image - the image to which you are adding noise
%     correlation - the correlation parameter of the gauss-markov process
%     mean - the mean of the iid sequence of Gaussian random variables
%     sigma - the variance of the iid sequence of Gaussian random variables
% 
% Output:
%     image - the noisified image
function image = add_gauss_markov_noise(image, correlation, mean, sigma)
    rng(0, 'twister'); % Choose your random generator to use randn()
    
    V = zeros(size(image));
    for k = 1:numel(V)
        noise = 0;
        if (k - 1) ~= 0
            noise = correlation * V(k - 1);
        end
        V(k) = noise + sigma * randn(1, 1) + mean;
    end
    
    % Add noise to image (transposed to get in rows)
    image = uint8(double(image) + V');
end