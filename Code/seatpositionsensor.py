import sys
from collections import Counter
import serial
import pandas as pd
import numpy as np
import openpyxl

from collections import Counter

import time

def distance_cal_fill(current_row, current_sensors):
    current_row[6] = (current_sensors[0] - current_row[0])**2 + (current_sensors[1] - current_row[1])**2 + (current_sensors[2] - current_row[2])**2 + (current_sensors[3] - current_row[3])**2 + (current_sensors[4] - current_row[4])**2


def distance_cal_fill_train(current_row, current_sensors):
    current_row[6] = (current_sensors[0] - current_row[0])**2 + (current_sensors[1] - current_row[1])**2 + (current_sensors[2] - current_row[2])**2 + (current_sensors[3] - current_row[3])**2 + (current_sensors[4] - current_row[4])**2


# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    ser = serial.Serial('COM3', baudrate=115200, timeout=1)

    # Give the location of the file
    path = "sensor_samples.xlsx"
    df = pd.read_excel(path, usecols="B:G", skiprows=0)
    new_column = np.full((df.shape[0], 1), -1)  # Create a new column filled with -1 values
    numpy_array_with_new_column = np.column_stack( (df, new_column))  # Stack the new column with the existing array

    # Convert DataFrame to NumPy array
    sensor_vector = [-1 for _ in range(5)]
    sample_vector = [[-1 for _ in range(7)],[-1 for _ in range(7)],[-1 for _ in range(7)],[-1 for _ in range(7)],[-1 for _ in range(7)],[-1 for _ in range(7)]]
    print("Choose Mode And Press Button To Start")
    print("To Switch Between Modes, Change The State Of The Switch")


    first_time = True
while 1:
    arduinoData = ser.readline().decode('ascii')


    if(first_time):
        while(len(arduinoData)==0):
            arduinoData = ser.readline().decode('ascii')
        if "Training Mode" in arduinoData:
            mode = 1
        else:
            mode = 0
        first_time = False;


    if(not mode):
        print("***************************Staring Predicting Mode***************************")
        print("Press The Button To Start the prediction")
        while 1:


            arduinoData = ser.readline().decode('ascii')
            if (len(arduinoData) > 0):
                if ("Training Mode" in arduinoData):
                    mode = 1
                    break


                print(arduinoData)
                if "the average of the sensor 1 values is  = " in arduinoData:
                    # print(arduinoData)
                    substring = arduinoData[
                                len("the average of the sensor 1 values is  = "):]  # Note that Python uses 0-based indexing
                    float_number = float(substring)
                    sensor_vector[0] = float_number

                if "the average of the sensor 2 values is  = " in arduinoData:
                    # print(arduinoData)
                    substring = arduinoData[
                                len("the average of the sensor 2 values is  = "):]  # Note that Python uses 0-based indexing

                    float_number = float(substring)
                    sensor_vector[1] = float_number

                if "the average of the sensor 3 values is  = " in arduinoData:
                    # print(arduinoData)
                    substring = arduinoData[
                                len("the average of the sensor 3 values is  = "):]  # Note that Python uses 0-based indexing

                    float_number = float(substring)
                    sensor_vector[2] = float_number

                if "the average of the sensor 4 values is  = " in arduinoData:
                    # print(arduinoData)
                    substring = arduinoData[
                                len("the average of the sensor 4 values is  = "):]  # Note that Python uses 0-based indexing

                    float_number = float(substring)
                    sensor_vector[3] = float_number
                if "the average of the sensor 5 values is  = " in arduinoData:
                    # print(arduinoData)
                    substring = arduinoData[
                                len("the average of the sensor 5 values is  = "):]  # Note that Python uses 0-based indexing


                    float_number = float(substring)


                    sensor_vector[4] = float_number
                    # import columns A and C from Excel file
                    for row in numpy_array_with_new_column :
                        distance_cal_fill(row, current_sensors=sensor_vector)


                    sorted_indices = np.argsort(numpy_array_with_new_column[:, 6])

                    # Reverse the sorted indices to achieve ascending order

                    # Reorder the rows of the matrix based on the sorted indices
                    sorted_matrix = numpy_array_with_new_column[sorted_indices]
                    # Specify the column index to analyze
                    column_index = 5  # 6th column index (0-indexed)

                    # Extract the top 10 rows of the 5th column
                    top_10_values = sorted_matrix[:10, column_index]

                    top_10_dis = sorted_matrix[:10, 6]
                    avg = sum(top_10_dis)/10

                    # Use Counter to count occurrences of each value
                    value_counts = Counter(top_10_values)

                    # Find the most common value
                    most_common_value = value_counts.most_common(1)[0][0]
                    if(most_common_value == 5 and sensor_vector[4]>0):
                        most_common_value = 1
                    if (most_common_value == 1 and sensor_vector[4] == 0):
                            most_common_value = 5
                    if (avg > 100):
                        most_common_value = 7
                    print(sensor_vector[4])
                    most_common_value_string = str(most_common_value)
                    print(f" ***************************The modules prediction is that you are setting  {most_common_value} ************************")
                    ser.write(bytes(most_common_value_string, 'utf-8'))
                    time.sleep(0.05)





    if mode == 1:
        numOfPosition = 1
        print("****************************Staring Training Mode****************************")
        flag = 0
        print("Press The Button To Start Recording Position Number 1")

        while 1:

            if(numOfPosition == 7):
                print("Sample Completed")
                break


            arduinoData = ser.readline().decode('ascii')
            if (len(arduinoData) > 0):
                if("Predict Mode" in arduinoData):
                    mode = 0
                    if(numOfPosition != 1):
                        print("Sampling Not Completed!")
                    break

                print(arduinoData)
                if "the average of the sensor 1 values is  = " in arduinoData:
                        # print(arduinoData)
                        substring = arduinoData[
                                    len("the average of the sensor 1 values is  = "):]  # Note that Python uses 0-based indexing
                        float_number = float(substring)
                        sample_vector[numOfPosition-1][0] = float_number

                if "the average of the sensor 2 values is  = " in arduinoData:
                        # print(arduinoData)
                        substring = arduinoData[
                                    len("the average of the sensor 2 values is  = "):]  # Note that Python uses 0-based indexing

                        float_number = float(substring)
                        sample_vector[numOfPosition-1][1] = float_number

                if "the average of the sensor 3 values is  = " in arduinoData:
                        # print(arduinoData)
                        substring = arduinoData[
                                    len("the average of the sensor 3 values is  = "):]  # Note that Python uses 0-based indexing

                        float_number = float(substring)
                        sample_vector[numOfPosition-1][2] = float_number

                if "the average of the sensor 4 values is  = " in arduinoData:
                        # print(arduinoData)
                        substring = arduinoData[
                                    len("the average of the sensor 4 values is  = "):]  # Note that Python uses 0-based indexing

                        float_number = float(substring)
                        sample_vector[numOfPosition-1][3] = float_number
                if "the average of the sensor 5 values is  = " in arduinoData:
                        # print(arduinoData)
                        substring = arduinoData[
                                    len("the average of the sensor 5 values is  = "):]  # Note that Python uses 0-based indexing

                        float_number = float(substring)

                        sample_vector[numOfPosition-1][4] = float_number
                        sample_vector[numOfPosition-1][5] = numOfPosition
                        # import columns A and C from Excel file
                        for row in numpy_array_with_new_column:
                            distance_cal_fill(row, current_sensors=sample_vector[numOfPosition-1])

                        sorted_indices = np.argsort(numpy_array_with_new_column[:, 6])

                        # Reverse the sorted indices to achieve ascending order

                        # Reorder the rows of the matrix based on the sorted indices
                        sorted_matrix = numpy_array_with_new_column[sorted_indices]

                        # Specify the column index to analyze
                        column_index = 5  # 6th column index (0-indexed)

                        # Extract the top 10 rows of the 5th column
                        top_10_values = sorted_matrix[:10, column_index]

                        # Use Counter to count occurrences of each value
                        value_counts = Counter(top_10_values)

                        # Find the most common value
                        most_common_value = value_counts.most_common(1)[0][0]
                        if (most_common_value == 5 and sample_vector[numOfPosition-1][4] > 0):
                            most_common_value = 1
                        if (most_common_value == 1 and sample_vector[numOfPosition-1][4]  == 0):
                            most_common_value = 5

                        if(most_common_value != numOfPosition):
                            print("Not Valid Position, press the button to try again postion number " ,numOfPosition,"!")
                        else:


                            # Adding the new row to the data set
                            if(numOfPosition == 6):
                                numpy_array_with_new_column = np.append(numpy_array_with_new_column, sample_vector, axis=0)
                                print(numpy_array_with_new_column)
                                new_df = pd.DataFrame(numpy_array_with_new_column)
                                new_df.to_excel(path,sheet_name="Sheet1", index = True, startrow = 0, startcol= 0)
                            numOfPosition = numOfPosition + 1
                            if(numOfPosition < 7):
                                print("Press The Button To Start Recording Position Number ", numOfPosition)





