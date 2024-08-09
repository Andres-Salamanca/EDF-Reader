#include "edf.hpp"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
const double PI = acos(-1);
void write_to_csv(const std::string& filename, const Eigen::VectorXcd& data) {
    std::ofstream file(filename);

    if (file.is_open()) {
        file << "Index,Real,Imaginary\n";
        for (int i = 0; i < data.size(); ++i) {
            file << i << "," << data[i].real() << "," << data[i].imag() << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}


int main (int argc, char *argv[]) {
  
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::cout << "Opening file ... " << argv[1] << std::endl;

    try {
        edf edfFile { std::string(argv[1]) };
        std::cout << "File opened and read successfully." << std::endl;
        edfFile.PrintHeaderRecords();
        edfFile.PrintDataRecords();
        edfFile.PrintSizeSignals();
        //edfFile.WriteRawCsv("signal.csv");
        edfFile.PrintTopValues(10);
        edfFile.Signals.CalculateDeviation();
        edfFile.Signals.PrintMeanAndDeviation();
        /*auto sum = edfFile.Signals.RuningSum(edfFile.Signals.Signals[0]);
        std::cout << "Sum size = " << sum.size() <<std::endl;
        for(const auto num : sum ){
          std::cout << num << " "; 
        }
        std::cout << std::endl;
        auto Diff = edfFile.Signals.FirstDifference(edfFile.Signals.Signals[0]);
        std::cout << "Diff size = " << Diff.size() <<std::endl;
        for(const auto di : Diff ){
          std::cout << di << " "; 
        }
        std::cout << std::endl;*/
        
        int N = 32768; // Number of samples
        double T = 1.0 / N; // Sampling period
        double freq = 2.0; // Frequency of the sine wave

        Eigen::VectorXcd sine_wave(N);

        // Generate sine wave
        for (int i = 0; i < N; ++i) {
            double t = i * T; // Time index
            sine_wave[i] = std::sin(2 * PI * freq * t); // Sine wave formula
        }

        // Write original sine wave to CSV
        write_to_csv("sine_wave_before_fft.csv", sine_wave);

        edfFile.Signals.FFT(sine_wave);

        // Write FFT result to CSV
        write_to_csv("sine_wave_after_fft.csv", sine_wave);

        edfFile.Signals.IFFT(sine_wave);

        write_to_csv("sine_wave_after_Ifft.csv", sine_wave);

        std::cout << "Sine wave data has been written to CSV files.\n";

    } catch (const std::runtime_error& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
  return 0;
}
