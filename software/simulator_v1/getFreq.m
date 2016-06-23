function [freqBin, value] = getFreq(whichFreq, binLabelsHz, fftAbs)  
  [minVal index] = min(abs(binLabelsHz-whichFreq));
  closestValue = fftAbs(index);

  % return
  freqBin = binLabelsHz(index);
  value = closestValue;
end