function out = test() 
  pkg load image
  
  % Load sample Image
  sampleImage = loadSampleImage("apple_100.png");  
  reconstruction = zeros(size(sampleImage));
  
  % Setup simulation
  numEpochs = 100;
  codeFieldMin = 40;    % 40uT
  codeFieldMax = 100;    % 55uT
  
  % Sampling frequency (radio)
  sampleFreq = 44100;        % 44.1kHz
  sampleDuration = 1;        % Seconds

  % Evaluation metric history  
  PSNRHistory = zeros(numEpochs, 1);  
  MSEHistory = zeros(numEpochs, 1);  
  
    
  for epoch = 1:numEpochs  
    disp(['epoch ' int2str(epoch)]);
  
    % Generate random code
    codeSampledSize = 10;
    codeSampled = generateRandomCode(codeSampledSize, codeFieldMin, codeFieldMax);   % 40 to 55uT
  
    % Interpolate random code
    codeInterpolationFactor = 10;
    codeInterpolated = interpolateCode(codeSampled, codeInterpolationFactor);
     
  
    % Generate waveform data
    t = (1/sampleFreq):(1/sampleFreq):sampleDuration;       % time steps
    signalSum = zeros(size(t));                             % initialize empty signal
    for x = 1:size(sampleImage, 1)
      for y = 1:size(sampleImage, 2)
        magStrength = codeInterpolated(x, y);
        amplitude = sampleImage(x, y);
        signalOnePixel = generateSignal(magStrength, amplitude, sampleFreq, sampleDuration);
        signalSum = signalSum .+ signalOnePixel;
      end
      disp(x);
      fflush(stdout);
    end
  
    % Frequency analysis on waveform
    [binsHz, fftAmp] = freqAnalysis(signalSum, sampleFreq);
  
  
    % Display  
    figure(1);
    clf();
    colormap('gray')
    
    subplot(2, 3, 1);
    imagesc(codeSampled); 
    axis image;
    title('code (simulated measurement, uT)');
    colorbar;
  
    subplot(2, 3, 2);  
    imagesc(codeInterpolated);
    axis image;
    title('code (interpolated, uT)');
    colorbar;
  
    subplot(2, 3, 4);  
    imagesc(sampleImage);
    axis image;
    title('Sample slice (ground truth)');
    colorbar;
    
    subplot(2, 3, 5);
    N_2 = ceil(size(binsHz, 2)/2);
    plot(binsHz(1:N_2), fftAmp(1:N_2));
    xlabel('Frequency (Hz)');    
    title('Signal (Frequency domain)');
    axis tight
  
    figure(2);
    clf();  
    plot(signalSum);
    title (['Signal (time domain, ' int2str(sampleFreq) ' samples/sec)']);
    xlabel('Time (sample index)');
    ylabel('Ampl.');
  
    % Reconstruction  
    for x = 1:size(codeInterpolated, 1)
      for y = 1:size(codeInterpolated, 2)      
        magStrength = codeInterpolated(x, y);
        freq = magStrength * 42.576;    % 42.576 Hz/uT for protons
        [freqBin value] = getFreq(freq, binsHz, fftAmp);
        reconstruction(x, y) += value;
      end
      disp(x);
      fflush(stdout);
    end
  
    % Display current reconstruction
    figure(1);
    subplot(2, 3, 6);
    imagesc(reconstruction);
    title(['reconstruction e' int2str(epoch)]);
    
    % Display current reconstruction (for animation export)
    figure(7);
    clf();

    colormap('gray')
    subplot(1, 3, 1);
    imagesc(sampleImage);
    axis off;
    title('Original');
    
    subplot(1, 3, 2);
    imagesc(codeInterpolated);
    axis off;
    title(['Coded Field (' num2str(codeFieldMin) 'uT to ' num2str(codeFieldMax) 'uT)']);

    subplot(1, 3, 3);
    imagesc(reconstruction);
    axis off;
    title(['Reconstruction (epoch ' num2str(epoch) ')']);
            
    % save animation for this epoch
    print(7, ['out' int2str(epoch) '.png'], '-S625,225', '-tight', '-FAriel:5');
  
  
    % Evalute error in reconstruction
    maxVal = max(max(reconstruction));
    reconstructionNorm = reconstruction ./ maxVal;
  
    squaredErrorReconstruction = (double(sampleImage) - double(reconstructionNorm)) .^ 2;
    % Display the squared error image.
    figure(5);  
    imshow(squaredErrorReconstruction, []);
    title('Squared Error Image');
    % Sum the Squared Image and divide by the number of elements
    % to get the Mean Squared Error.  It will be a scalar (a single number).
    mse = sum(sum(squaredErrorReconstruction)) / (size(sampleImage, 1) * size(sampleImage, 2));
    % Calculate PSNR (Peak Signal to Noise Ratio) from the MSE according to the formula.
    PSNR = 10 * log10( 1 / mse);
    PSNRHistory(epoch) = PSNR;
    MSEHistory(epoch) = mse;
    
    % Alert user of the answer.  
    disp (['PSNR: ' num2str(PSNR)]);
  
  
    figure(1);
    subplot(2, 3, 3);
    plot(MSEHistory);
    title('MSE vs Epoch');
  
  
  
  
    % Force update of figures  
    drawnow;
  
  end

  % return
  out = reconstruction;

end