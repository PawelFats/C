import csv
import numpy as np
import matplotlib.pyplot as plt

# Считываем данные из CSV файла
results = []
with open('benchmark_results.csv', newline='') as csvfile:
    reader = csv.reader(csvfile)
    for row in reader:
        if len(row) >= 10:
            task_name = row[1]
            avg_time = float(row[9])
            task_perf = float(row[10])
            results.append((task_name, avg_time, task_perf))

# Словарь для хранения производительности по каждой задаче
task_performance = {}

# Группировка по задачам
for task_name, avg_time, task_perf in results:
    if task_name not in task_performance:
        task_performance[task_name] = []
    task_performance[task_name].append(task_perf)

# Вычисляем среднюю производительность для каждой задачи
avg_performance = {task: np.mean(perfs) for task, perfs in task_performance.items()}

# Построение столбчатой диаграммы
tasks = list(avg_performance.keys())
performance_values = list(avg_performance.values())

plt.figure(figsize=(10, 6))
plt.bar(tasks, performance_values, color='skyblue')
plt.xlabel('Task')
plt.ylabel('Average Performance (TaskPerf)')
plt.title('Average Performance for Each Task')
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()

# Оценка среднего быстродействия при равновероятном использовании всех задач
overall_avg_performance = np.mean(performance_values)
print(f"Средняя производительность при равновероятном использовании задач: {overall_avg_performance}")
