%% Compute Error
% Determines the difference between an original and restored image, each
% of the same size and determines the probability of error.
%
% Inputs:
% source - the original image
% restored - the restored image
%
% Returns:
% error - the probability of error
function error = compute_error(source, restored)

    total_errors = 0;
    for i = 1:size(source, 1)
        for j = 1:size(source, 2)
            if source(i,j) ~= restored(i,j)
                total_errors = total_errors + 1;
            end
        end
    end
    
    error = total_errors / (size(source, 1)*size(source, 2));
end