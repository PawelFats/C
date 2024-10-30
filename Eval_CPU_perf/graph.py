import pandas as pd
import matplotlib.pyplot as plt

# Установка стиля графиков
plt.style.use('seaborn-v0_8-darkgrid')

# Чтение данных из CSV файла
data = pd.read_csv('/home/nazyva/gitOK/C/Eval_CPU_perf/benchmark_results.csv', header=None)

# Задаем названия столбцов для удобства
data.columns = [
    'Processor', 'Task', 'Type', 'Optimization', 'InsCount', 
    'TimingMethod', 'MatSaize', 'Trials', 'AvgTime', 'StdDev', 'StdDevPercent', 'Performance'
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
