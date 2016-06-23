function [binLabelsHz, fftAbs] = freqAnalysis(sumSignal, sampleFreq)
  N = length(sumSignal)  
  fnyquist = sampleFreq/2;
  fftAbs = abs(fft(sumSignal));
  binVals = [0 : N-1];
  binLabelsHz = binVals*(sampleFreq/N);
  N_2 = ceil(N/2);
  
  figure(3);
  plot(binLabelsHz(1:N_2), fftAbs(1:N_2))
  xlabel('Frequency (Hz)')
  ylabel('Ampl.');
  title('Signal (Frequency domain)');
  axis tight

end