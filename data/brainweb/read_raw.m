function I = read_raw(fn,shape)

% Read given file
fid = fopen(fn, 'r');
I = fread(fid);

% Check shape
if ~exist('shape', 'var')
    d = sqrt(size(I,1));
    shape = [d d];
end

% Reshape into image
I = reshape(I', shape);
fclose(fid);

end
