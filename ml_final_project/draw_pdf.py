import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn import neighbors
from sklearn import tree 

def preprocessing():
    df = pd.read_csv('car.csv')
    
    #delete unnecessary data columns
    df = df.drop(df.index[0]) #drop the label, say the first row , NOT IN PLACE
    del df['body_type']
    del df['color_slug']
    del df['stk_year']
    del df['date_created']
    del df['date_last_seen']
    df = df.dropna() #drop row with na value, NOT IN PLACE
    
    #print(df)
    car_name = df[['maker']]
    car_name.dtypes.value_counts()
    return df

def draw_PDF(df): #draw the pdf of each column
    df = df.mask(df.astype(object).eq('None')).dropna()
    sample_df = df.sample(n=1000)
    
    print(sample_df)
    maker_count = sample_df.groupby(['maker']).size()
    
    year_count = sample_df.groupby(['manufacture_year']).size()
    
    fuel_count = sample_df.groupby(['fuel_type']).size()
    
    sample_df['door_count'] = sample_df['door_count'].astype(int)
    
    door_count = sample_df.groupby(['door_count']).size()
    print(door_count)
    
    makers = maker_count.keys()
    maker_list = []
    count_list = []
    summ = 0
    for i in range(len(makers)):
        maker_list.append(makers[i])
        count_list.append(maker_count[i])
        summ = summ + maker_count[i]
        
    years = year_count.keys()
    year_count = year_count.tolist()
    #print(years)
    year_list = []
    year_count_list = []
    year_sum = 0
    for i in range(len(years)):
        year_list.append(years[i])
        year_count_list.append(year_count[i])
        year_sum = year_sum + year_count[i]
    
    fuel = fuel_count.keys()
    fuel_list = []
    fuel_count_list = []
    fuel_sum = 0
    for i in range(len(fuel)):
        fuel_list.append(fuel[i])
        fuel_count_list.append(fuel_count[i])
        fuel_sum = fuel_sum + fuel_count[i]
    
    doors = door_count.keys()
    door_count = door_count.tolist()
    door_list = []
    door_count_list = []
    door_sum = 0
    for i in range(len(doors)):
        door_list.append(doors[i])
        door_count_list.append(door_count[i])
        door_sum = door_sum + door_count[i]
    
    
    #print(year_list)
    #print(year_count_list)
    
    plt.figure(figsize=(30,5))
    plt.plot(maker_list,count_list/summ)
    plt.tight_layout()
    plt.show()
    
    #plt.figure(figsize=(10,5))
    #print(type(year_list))
    #print((year_count_list))
    for k in range(len(year_count_list)):
        year_count_list[k] = year_count_list[k]/year_sum
    plt.plot(year_list,year_count_list)
    plt.xticks(np.arange(min(year_list), max(year_list)+1, 5.0))
    plt.tight_layout()
    plt.show()
    
    plt.plot(fuel_list,fuel_count_list/fuel_sum)
    plt.tight_layout()
    plt.show()
    
    plt.xticks(np.arange(min(door_list), max(door_list)+1, 1.0))
    for k in range(len(door_count_list)):
        door_count_list[k] = door_count_list[k]/door_sum
    plt.plot(door_list,door_count_list)
    plt.tight_layout()
    plt.show()
        
if __name__ == '__main__':
    draw_PDF(preprocessing())
