import pandas as pd
import matplotlib.pyplot as plt

# Загрузка данных из CSV файла
csv_file = 'results.csv'  # Укажите путь к вашему CSV файлу
data = pd.read_csv(csv_file, sep=';')

# Определяем заголовки столбцов
headers = [
    "memoryType", "blockSize", "elementType", "bufferSize", 
    "launchNum", "timer", "writeTime", "avgWriteTime", 
    "writeBandwidth", "absErrorWrite", "relErrorWrite", 
    "readTime", "avgReadTime", "readBandwidth", 
    "absErrorRead", "relErrorRead"
]

# Добавляем заголовки
data.columns = headers

data['blockSize'] = data['blockSize'] / 1024

# 1. Зависимость пропускной способности записи и чтения от размера блока данных
def plot_bandwidth_vs_blocksize(data):
    for mem_type in data['memoryType'].unique():
        plt.figure(figsize=(12, 6))
        
        # Группируем данные по 'blockSize' и берем максимальное значение пропускной способности
        subset = data[data['memoryType'] == mem_type]
        subset = subset.groupby('blockSize').agg({
            'writeBandwidth': 'max', 
            'readBandwidth': 'max'
        }).reset_index()
        
        plt.plot(subset['blockSize'], subset['writeBandwidth'], label=f'Запись - {mem_type}')
        plt.plot(subset['blockSize'], subset['readBandwidth'], label=f'Чтение - {mem_type}')

        plt.title(f'Зависимость пропускной способности от размера блока данных для {mem_type}')
        plt.xlabel('Размер блока данных (BlockSize Kb)')
        plt.ylabel('Пропускная способность (Bandwidth)')
        plt.legend()
        plt.grid()
        
        plt.savefig(f'{mem_type}_bandwidth_vs_blocksize.png')
        plt.close()



# 2. Зависимость погрешности измерения пропускной способности от размера блока данных
def plot_error_vs_blocksize(data): 
    for mem_type in data['memoryType'].unique():
        plt.figure(figsize=(12, 6))

        subset = data[data['memoryType'] == mem_type]
        
        # Группируем данные по 'blockSize' и берем максимальное значение погрешности
        subset = subset.groupby('blockSize').agg({
            'absErrorWrite': 'max', 
            'absErrorRead': 'max'
        }).reset_index()
        
        plt.plot(subset['blockSize'], subset['absErrorWrite'], label=f'Абсолютная погрешность записи - {mem_type}')
        plt.plot(subset['blockSize'], subset['absErrorRead'], label=f'Абсолютная погрешность чтения - {mem_type}')

        plt.title('Зависимость погрешности измерения от размера блока данных')
        plt.xlabel('Размер блока данных (BlockSize Kb)')
        plt.ylabel('Погрешность (Absolute Error)')
        plt.legend()
        plt.grid()
        plt.savefig(f'{mem_type}_error_vs_blocksize.png')
        plt.close()

# 3. Зависимость погрешности измерений от числа испытаний
def plot_error_vs_launchnum(data):
    for mem_type in data['memoryType'].unique():
        plt.figure(figsize=(12, 6))
    
        subset = data[data['memoryType'] == mem_type]
        
        # Группируем данные по 'launchNum' и берем максимальное значение погрешности
        subset = subset.groupby('launchNum').agg({
            'absErrorWrite': 'max', 
            'absErrorRead': 'max'
        }).reset_index()
        
        plt.plot(subset['launchNum'], subset['absErrorWrite'], label=f'Абсолютная погрешность записи - {mem_type}')
        plt.plot(subset['launchNum'], subset['absErrorRead'], label=f'Абсолютная погрешность чтения - {mem_type}')

        plt.title('Зависимость погрешности измерений от числа испытаний')
        plt.xlabel('Число испытаний (LaunchNum)')
        plt.ylabel('Погрешность (Absolute Error)')
        plt.legend()
        plt.grid()
        plt.savefig(f'{mem_type}_error_vs_launchnum.png')
        plt.close()

# Построение графиков
plot_bandwidth_vs_blocksize(data)
plot_error_vs_blocksize(data)
plot_error_vs_launchnum(data)

print("Графики успешно созданы и сохранены.")
