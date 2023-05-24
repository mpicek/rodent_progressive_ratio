# Progressive Ratio Task for Rodents

Source code for David Levčík's research at the Czech Academy of Sciences.
In this task, a rodent is placed in a specialized cage with a button that gives him reward for pushing it. However, the number of pushes necessary for obtaining a reward increases through time. The output of this experiment is to measure the rodent's will to push the button to get the reward.

In our project, this experiment is supposed to measure how evolved is the Alzheimer's disease in rodent models we use. It is known that with progressed Alzheimer's disease, the subject does not feel pleasure as much as a healthy subject, therefore is not that motivated to push the button.

## How to run the code

As our Arduino circuitry is standardized in our lab, it is only necessary to upload the code into Arduino.

Then, open the command line and navigate into the project's folder with `control.py`:
```cd my_subdirectory``` changes directory to my_subdirectory.
```cd ..``` changes directory to the parent directory.

To list the files and subdirectories in the current directory, write `dir`.
To print the current path, write `cd` with no other parameters.

When you navigate into the correct folder, run `python control.py`. It detects the port with Arduino and controls the experiment.

