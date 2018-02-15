function var_v = myvar(I)
I = I(:);
var_v = sum((I-mean(I)).^2)/numel(I);
end