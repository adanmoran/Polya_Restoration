%% Add Bursty Noise to an Image
% This image will add bursty noise (streaks) to an image.
%
% Inputs: 
% image - the image to which you are adding noise
% transition_prob - a value between 0 and 1 which denotes the 
% markov probability of streaking (i.e. P(Xn == Xn-1 | Xn-1) )
% error - the bit error rate
% colour - the colour of the image. 
%          Options: 
%          * 'binary' for "full on or off" pixels
%          * 'gaussian' for a gauss-markov style of noise
% mean - the mean value of noise. Only works for gaussian noise. Default 0.
% sigma - the standard deviation. Only works for gaussian noise. Default 1.
% Outputs:
% image - the noisified image
function image = add_bursty_noise( ...
    image, ...
    transition_prob, ...
    error, ...
    noise, ...
    mean, ...
    sigma ...
)
    switch(nargin)
        case 3
            noise = 'binary';
        case 4
            if ~strcmp(noise,'binary') ...
                && ~strcmp(noise,'gaussian')
                error('Can only add binary or gaussian burst noise.');
            end
            if strcmp(noise, 'gaussian')
                mean = 0;
                sigma = 1.0;
            end
        case 6
            if ~strcmp(noise, 'gaussian')
                error('Can only use mean and sigma with gaussian noise.');
            end
        case 5
            if ~strcmp(noise, 'gaussian')
                error('Can only use mean and sigma with gaussian noise.');
            end
            sigma = 1.0;
        otherwise
            error('Not enough parameters');
    end
    rng(0, 'twister'); % Choose your random generator to use randn()
    % Iterate through the rows and add bursty noise to each row
    prev = 0;
    for i = 1:size(image,1)
        for j = 1:size(image,2)
            % By default, we do not flip bits
            val = 0;
            % if we flipped the last time, flip again with
            % probability given by transition_prob coefficient
            switch(prev)
                % If we flipped a bit the last time, flip one again based
                % on the transition_prob draw
                case 1
                    prob_0 = 1 - transition_prob;
                    val = randsample([0,1],1,true,[prob_0, transition_prob]);
                % If we didn't flip a bit last time...
                otherwise
                    % See if a bit should be flipped based on
                    % transition_prob
                    prob_1 = 1 - transition_prob;
                    tmp = randsample([0,1],1,true,[transition_prob, prob_1]);
                    % Then see if the bit should be flipped based on the
                    % error draw
                    flip = randsample([0,1],1,true,[1-error, error]);
                    % If both transition_prob and error say to flip, flip the
                    % bit
                    if tmp == 1 && flip == 1
                        val = 1;
                    end
            end
            
            % If we drew a 1 we add noise (flip pixel or add gauss noise)
            if val == 1
                newColour = 0;
                if strcmp(noise, 'binary')
                    if image(i,j) == 0
                        newColour = 1;
                    end
                else
                    newColour = image(i,j) + sigma*randn(1,1) + mean;
                end
                % If our image is 3D (i.e. layered grayscales), assign the
                % new colour to each
                
                if length(size(image)) == 3
                    for n = 1 : size(image,3)
                        image(i,j,n) = newColour;
                    end
                else
                    image(i,j) = newColour;
                end
            end
            % Keep track of this current value in order to check whether we
            % should flip again the next time
            prev = val;
        end 
    end
end