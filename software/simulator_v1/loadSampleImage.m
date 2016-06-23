function out = loadSampleImage(filename)
  sampleImage = double(rgb2gray(imread(filename)));
  sampleImage = sampleImage ./ 255;             % normalize (0-1)
  
  % return 
  out = sampleImage;
end