function out = interpolateCode(codeIn, scale)
  codeClass = class(codeIn);
  codeSize = size(codeIn, 1);
  interpolateDelta = 1/(scale+1) * ((codeSize*scale) / ((codeSize*scale)+1));
  [x y] = meshgrid(1:codeSize);
  [xi yi] = meshgrid(1:interpolateDelta:codeSize);
  
  interpolatedCode = cast(interp2(x, y, double(codeIn), xi, yi, 'cubic'), codeClass);
  
  % Return
  out = interpolatedCode;
end