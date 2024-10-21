import pandas as pd
import matplotlib.pyplot as plt
import random

# Leer el archivo CSV
df = pd.read_csv('gant.csv')

# Identificar los bloques consecutivos para cada proceso
gantt_data = []
start_tick = df.iloc[0]['Tick']
current_pid = df.iloc[0]['Process']

ticks_cambio = [start_tick]  # Lista de ticks donde hay cambios de proceso

for i in range(1, len(df)):
    tick = df.iloc[i]['Tick']
    pid = df.iloc[i]['Process']

    if pid != current_pid:  # Guardar bloque anterior y registrar cambio
        gantt_data.append((current_pid, start_tick, tick - start_tick))
        ticks_cambio.append(tick)  # Registrar tick de cambio
        start_tick = tick
        current_pid = pid

# Añadir el último bloque
gantt_data.append((current_pid, start_tick, df.iloc[-1]['Tick'] - start_tick + 1))

# Ajustar el tamaño de la figura dinámicamente
fig_width = max(10, len(df) / 2)  # Escalar ancho con el número de ticks
fig_height = max(6, len(df['Process'].unique()) * 0.6)  # Altura según PIDs
fig, ax = plt.subplots(figsize=(fig_width, fig_height))

# Función para generar colores pastel
def pastel_color():
    r = (random.random() + 1) / 2  # Mezclar con blanco para suavizar
    g = (random.random() + 1) / 2
    b = (random.random() + 1) / 2
    return (r, g, b)

# Asignar un color pastel diferente a cada PID
unique_pids = df['Process'].unique()
colors = {pid: pastel_color() for pid in unique_pids}

# Dibujar las barras horizontales para cada bloque
for pid, start, duration in gantt_data:
    ax.barh(
        y=pid,                          # PID en el eje Y
        width=duration,                 # Duración del bloque
        left=start,                     # Tick de inicio
        color=colors[pid],              # Color pastel asignado
        edgecolor='black'               # Borde para mejor visibilidad
    )

# Configurar los ejes y las etiquetas
ax.grid()
ax.set_title('Diagrama de Gantt')
ax.set_xlabel('Tick')
ax.set_ylabel('Process (PID)')
ax.set_yticks(unique_pids)  # Mostrar solo los PIDs únicos en el eje Y
ax.set_xticks(ticks_cambio)  # Mostrar solo los ticks de cambio en el eje X
ax.grid(axis='x', linestyle='--', alpha=0.7)  # Líneas verticales de referencia

# Dejar un margen adecuado entre las etiquetas del eje Y
ax.set_ylim(min(unique_pids) - 0.5, max(unique_pids) + 0.5)

plt.tight_layout()  # Ajustar automáticamente el layout para evitar recortes
plt.show()