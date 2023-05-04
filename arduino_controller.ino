//set time delays variables
unsigned long delay_motor = 10; // 10ms
unsigned long delay_gate = 2; // 2 ms
unsigned long delay_output = 50; // 50 ms
unsigned long delay_button = 70; // 50 ms

unsigned long delayStart_motor = 0;
unsigned long delayStart_gate = 0;
unsigned long delayStart_output = 0;
unsigned long last_button_press = 0;

bool delayRunning = false;

//set pins
//input values
int button = 6;
int gate = 7;

//gate source voltage
int gate_src = 13;

//stepper motor(a, a-, b, b-)
int a = 8; int aM = 9; int b = 11; int bM = 10;

//create variables for output com
int give_pellet;
int button_pressed; // pressed = 1, not pressed yet = 0
int button_state; // here we read the value from digitalRead

//crate variable to read motor values from com
int move_motor = 0;

//array for motor pin 0/1 movement
int pinsVal [25];
int steps = 0;
int motor_direction = 1;
int check_motor;


void setup() {
  
  //initialize pin values
  pinMode(button,INPUT);
  digitalWrite(button, HIGH);

  pinMode(gate,INPUT);
  digitalWrite(gate, HIGH);

  pinMode(gate_src, OUTPUT);
  digitalWrite(gate_src, HIGH); //connected as gate source, must be high

  //set motor pins
  pinMode(a, OUTPUT);
  digitalWrite(a, LOW);

  pinMode(aM, OUTPUT);
  digitalWrite(aM, LOW);

  pinMode(b, OUTPUT);
  digitalWrite(b, LOW);

  pinMode(bM, OUTPUT);
  digitalWrite(bM, LOW);

  //start timers for time delays
  delayRunning = true;
  delayStart_motor = millis();
  delayStart_output = millis();
  delayStart_output = millis();

  //begin serial com
  Serial.begin(9600);

}

void set_direction(){
  //create array of values for each step; each 4 values = 1 step; last 4 = to set 0 on each pin
  for (int i = 0; i < 25; i++){
    pinsVal[i] = 0;
    }
    //CCW rotation
    if (motor_direction == 1){
      pinsVal [0] = 1;
      pinsVal [7] = 1;
      pinsVal [9] = 1;
      pinsVal [14] = 1;
      }
     //CW rotation
     else if (motor_direction == 2){
      pinsVal [2] = 1;
      pinsVal [5] = 1;
      pinsVal [11] = 1;
      pinsVal [12] = 1;
     }
}

void do_step(){
  //set list of values to be sent to pins
  set_direction();
  digitalWrite(a, pinsVal[steps*4]);
  digitalWrite(aM, pinsVal[(steps*4) + 1]);
  digitalWrite(b, pinsVal[(steps*4) + 2]);
  digitalWrite(bM, pinsVal[(steps*4) + 3]);
}

void motor_handler(){
  if ((millis() - delayStart_motor) >= delay_motor) {
      delayStart_motor += delay_motor;
      
      if (move_motor){
        do_step();
        steps++;
        if (steps > 3) steps = 0; // cycle is starting again (stepping a motor has 4 steps defined in pinVal)
      }
   }
}

void button_handler(){
  if ((millis() - last_button_press) >= delay_button){
    button_state = digitalRead(button);
    button_state = !button_state; // because button logic inversed

    if (button_pressed && button_state == 0){ // pressed previously, now released, TODO: jakej je stav pri zmacknuti?? 0/1
      last_button_press = millis();
      Serial.print(millis());
      Serial.println(";button released");
      button_pressed = 0;
    }
    
    if (!button_pressed && button_state == 1){ // not pressed and now it was pressed
      button_pressed = 1;
      last_button_press = millis();
    }
  }
}

void serial_command_check(){
    if (Serial.available()){
      int read_val = Serial.read();
      if (read_val == 's'){ // mark experiment star
        Serial.print(millis());
        Serial.println(";start");
      }
      if (read_val == 'p'){ // give pellet
        give_pellet = 1;
        move_motor = 1;
      }
      if (read_val == 'e'){ // mark experiment end
        Serial.print(millis());
        Serial.println(";end");
    }
  }
}

void optic_gate_checker(){
  if ((millis() - delayStart_gate) >= delay_gate) {
      delayStart_gate += delay_gate;
  
      if (give_pellet != 0 && move_motor != 0){
        if (digitalRead(gate) == 0){
          give_pellet = 0;
          Serial.print(millis());
          Serial.println(";reward");
          //stop motor if reward was delivered
          steps = 5; //sends 0 to all pins .. in pinVal on these positions are just zeros
          do_step();
          move_motor = 0;
          steps = 0;
        }
      }
  }
}

void loop() {
  optic_gate_checker();
  motor_handler();
  button_handler();
  serial_command_check();
}
