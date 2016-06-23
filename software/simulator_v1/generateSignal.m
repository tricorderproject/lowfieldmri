function out = generateSignal(magStrength, amplitude, sampleFreq, duration)
  t = (1/sampleFreq):(1/sampleFreq):duration;
  freq = magStrength * 42.576;    % 42.576 Hz/uT for protons
  signal = amplitude * sin(2*pi*freq*t);
  
  % TODO: Add in exponential decay

  % return
  out = signal;
end