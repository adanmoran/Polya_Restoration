%Borrowed and adapted from Aaron Johnsy, February 2017
%Source: https://angeljohnsy.blogspot.com/2014/08/lee-filter.html

function var_v = myvar(I)
I = I(:);
var_v = sum((I-mean(I)).^2)/numel(I);
end