function out = generateRandomCode(size, min, max)
  delta = max - min;
  code = rand(size, size);
  code = code * delta;
  code = code + min;
  
  % Return
  out = code;
end