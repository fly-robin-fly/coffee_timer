template <size_t SIZE>
class VoltageSmoother {
private:
    float buffer[SIZE] = {0};
    size_t index = 0;
    size_t count = 0;

public:
    // Insert a new voltage reading into the buffer
    void add(float value) {
        buffer[index] = value;
        index = (index + 1) % SIZE; // Wrap around
        if (count < SIZE) count++;  // Track fill state
    }

    // Calculate and return the moving average
    float getAverage() const {
        if (count == 0) return 0.0f;
        
        float sum = 0.0f;
        for (size_t i = 0; i < count; i++) {
            sum += buffer[i];
        }
        return sum / count;
    }
    
    // Optional: Reset the buffer
    void reset() {
        index = 0;
        count = 0;
    }
};