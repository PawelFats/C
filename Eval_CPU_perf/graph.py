import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Установка стиля графиков
plt.style.use('seaborn-v0_8-darkgrid')

# Чтение данных из CSV файла
data = pd.read_csv('/home/pin-service/projects/labs/C/Eval_CPU_perf/benchmark_results.csv', header=None)

# Задаем названия столбцов для удобства
data.columns = [
    'Processor', 'Task', 'Type', 'Optimization', 'InsCount', 
    'TimingMethod', 'MatSize', 'Trials', 'AvgTime', 'StdDev', 'StdDevPercent', 'Performance'
]

# Преобразование столбца AvgTime в числовой формат
data['AvgTime'] = pd.to_numeric(data['AvgTime'], errors='coerce')

# Группировка данных по задачам и типам
performance_summary = data.groupby(['Task', 'Type'])['Performance'].mean().unstack()

# Построение графиков для каждой задачи
for task in performance_summary.index:
    fig, ax = plt.subplots(figsize=(10, 6))  # Создание графика с заданным размером
    bars = performance_summary.loc[task].plot(kind='bar', ax=ax, color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728'])
    
    # Настройка графика
    ax.set_title(f'Производительность для {task}', fontsize=16)
    ax.set_xlabel('Тип данных', fontsize=14)
    ax.set_ylabel('Средняя производительность (инструкций в секунду)', fontsize=14)
    ax.tick_params(axis='x', rotation=45)
    ax.grid(axis='y', linestyle='--', alpha=0.7)  # Добавление сетки

    # Добавление аннотаций с значениями производительности на столбцах
    for bar in bars.patches:
        ax.annotate(f'{bar.get_height():.2f}', 
                    (bar.get_x() + bar.get_width() / 2, bar.get_height()), 
                    ha='center', va='bottom', fontsize=10)

    plt.tight_layout()
    plt.savefig(f'gg_{task}.png')  # Сохранение графика с именем задачи
    plt.clf()  # Очистка графика для следующего

# Оценка среднего быстродействия для равновероятного использования типовых задач
mean_performance = performance_summary.mean().mean()  # Средняя производительность по всем задачам и типам

# Печать результата
print(f'Средняя производительность для равновероятного использования типовых задач: {mean_performance:.2f} инструкций в секунду')

# Построение сравнительных графиков по значениям оптимизации и типам, если значений больше одного
if data['Optimization'].nunique() > 1:
    tasks = data['Task'].unique()  # Получаем список задач
    
    for task in tasks:
        task_data = data[data['Task'] == task]  # Фильтруем данные для каждой задачи
        
        # Построение графика только если в задаче несколько значений Optimization
        if task_data['Optimization'].nunique() > 1:
            plt.figure(figsize=(12, 7))
            
            # Построение столбчатого графика с помощью seaborn
            sns.barplot(
                data=task_data, x='Optimization', y='AvgTime', hue='Type', 
                palette='viridis', edgecolor='black'
            )

            # Настройка графика
            plt.title(f'Среднее время выполнения для {task} по оптимизациям и типам данных', fontsize=16)
            plt.xlabel('Оптимизация', fontsize=14)
            plt.ylabel('Среднее время выполнения (сек)', fontsize=14)
            plt.xticks(rotation=45)
            plt.legend(title='Тип данных')
            plt.grid(axis='y', linestyle='--', alpha=0.7)

            plt.tight_layout()
            plt.savefig(f'comparison_{task}.png')  # Сохранение графика с именем задачи
            plt.clf()  # Очистка графика для следующего
