%% Add Bursty Noise to an Image
% This image will add bursty noise (streaks) to an image.
%
% Inputs: 
% image - the image to which you are adding noise
% correlation - a value between 0 and 1 which denotes the 
% markov probability of streaking (i.e. P(Xn == Xn-1 | Xn-1) )
% error - the bit error rate
% 
% Outputs:
% image - the noisified image
function image = add_bursty_noise(image, correlation, error)
    rng(0, 'twister'); % Choose your random generator to use randn()
    % Iterate through the rows and add bursty noise to each row
    prev = 0;
    for i = 1:size(image,1)
        for j = 1:size(image,2)
            % By default, we do not flip bits
            val = 0;
            % if we flipped the last time, flip again with
            % probability given by correlation coefficient
            switch(prev)
                % If we flipped a bit the last time, flip one again based
                % on the correlation draw
                case 1
                    prob_0 = 1 - correlation;
                    val = randsample([0,1],1,true,[prob_0, correlation]);
                % If we didn't flip a bit last time...
                otherwise
                    % See if a bit should be flipped based on
                    % correlation
                    prob_1 = 1 - correlation;
                    tmp = randsample([0,1],1,true,[correlation, prob_1]);
                    % Then see if the bit should be flipped based on the
                    % error draw
                    flip = randsample([0,1],1,true,[1-error, error]);
                    % If both correlation and error say to flip, flip the
                    % bit
                    if tmp == 1 && flip == 1
                        val = 1;
                    end
            end
            
            % If we drew a 1 we flip the pixel.
            if val == 1
                if image(i,j) > 0
                    image(i,j) = 0;
                else
                    image(i,j) = 1;
                end
            end
            % Keep track of this current value in order to check whether we
            % should flip again the next time
            prev = val;
        end 
    end
end