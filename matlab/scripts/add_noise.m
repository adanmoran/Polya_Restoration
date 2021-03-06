%% Helper Function to Add Noise Type
% Add the correct type of noise based on preferences object.
% 
% Inputs:
%     image - the image to which you are adding noise
%     prefs - the preferences structure containing prefs.image.type
%     noise - the noise structure containing parameters for the noise adding
%             functions
% 
% Outputs:
%     noisy_image - the noisified image
function noisy_image = add_noise(image, prefs, noise)
    % Reset the random generator so we always have the same noise
    rng(0, 'twister');
    noisy_image = image;
    
    for type = noise.type       
        if strcmp(type, 'speckle')
            % This is actually gaussian in the 'bw' case, not speckle
            if strcmp(prefs.image.type, 'bw')
                noisy_image = add_gaussian_noise( ...
                                    noisy_image, ...
                                    noise.bw.gaussian_std_dev, ...
                                    noise.bw.gaussian_mean, ...
                                    noise.bw.gaussian_confidence_interval ...
                                );

            else
                if strcmp(noise.speckle.distribution, 'rayleigh')
                    % Convert the variance into a rayleigh parameter
                    % according to the function for rayleigh variance
                    % Var[X] = (4 - pi)/2 * parameter^2
                    parameter = sqrt(( 2*noise.speckle.sigma) / (4 - pi));
                    % Create rayleigh noise with the given parameter
                    rayleigh = raylrnd(parameter, size(noisy_image));
                    % Shift the rayleigh noise to be mean 1
                    % Note that the mean of the rayleigh distribution is
                    % E[X] = sqrt(pi/2) * parameter
                    rayleigh = rayleigh + (1 - sqrt(pi/2)*parameter);
                    % Apply multiplicative noise to the image pointwise
                    noisy_image = uint8(double(noisy_image) .* rayleigh);
                else
                    % Apply uniformly-distributed muultiplicative speckle
                    noisy_image = imnoise(noisy_image, ...
                                          'speckle', ...
                                          noise.speckle.sigma);
                end
            end

        elseif strcmp(type, 'binary-erasure')
            noisy_image = add_bursty_noise(noisy_image, ...
                                           noise.bursty.transition_prob,...
                                           noise.bursty.error, ...
                                           'binary');
        elseif strcmp(type, 'gaussian-burst')
            noisy_image = add_bursty_noise(noisy_image, ...
                                           noise.bursty.transition_prob,...
                                           noise.bursty.error, ...
                                           'gaussian', ...
                                           noise.bursty.mean, ...
                                           noise.bursty.sigma);
        
        elseif strcmp(type, 'gauss-markov')
            noisy_image = add_gauss_markov_noise(noisy_image, ...
                                        noise.gauss_markov.correlation, ...
                                        noise.gauss_markov.mean, ...
                                        noise.gauss_markov.sigma);
        
        else % no noise if 'none' or an incorrect input
            fprintf('nothing');
            noisy_image = image;
            return;
        end
    end
end