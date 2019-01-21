class Button {
  private:
    	bool buttonState;
    	bool prevButtonState;
    	int counter;
    	unsigned int buttonPin;
    	int counterUpperLimit;
    	int counterLowerLimit;
  public:
      Button(unsigned int buttonPin, int counterLimit);
      bool transitionOccured();
      void updateButtonState(bool pinRead);
      bool getButtonState();
      bool isPushed();
      bool isReleased();
};
