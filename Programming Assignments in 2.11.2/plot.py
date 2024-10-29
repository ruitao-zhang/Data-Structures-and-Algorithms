import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from scipy.interpolate import CubicSpline

def plot_11(prefix, ns, title):
    plt.figure()
    for n in ns:
        filename = f"{prefix}{n}.txt"
        data = pd.read_csv(filename, sep='\t')
        plt.plot(data['x'], data['p_n(x)'], label=f'n={n}')
    plt.plot(data['x'], data['f(x)'], 'k--', label='Exact Function')  
    plt.legend()
    plt.title(title)
    plt.xlabel("x")
    plt.ylabel("f(x) or p_n(x)")
    plt.show()

# Runge 
plot_11("runge_data_n", [2, 4, 6, 8], "Runge Phenomenon Interpolation")

# Chebyshev
plot_11("chebyshev_data_n", [5, 10, 15, 20], "Chebyshev Interpolation")

# Heart
def heart_curve(n_points):
    x = np.linspace(-np.sqrt(3), np.sqrt(3), n_points)
    y_1 = (np.sqrt(3 - x**2) + np.sqrt(np.abs(x))) / 1.5
    y_0 = (-np.sqrt(3 - x**2) + np.sqrt(np.abs(x))) / 1.5
    points_1 = np.column_stack((x, y_1))
    points_0 = np.column_stack((x, y_0))
    return np.concatenate((points_1, points_0[::-1]), axis=0)

def Tangents(points):
    tangents = np.gradient(points, axis=0)
    return tangents

def bezier_points(p0, p1, tangent):
    q0 = p0
    q1 = p0 + (1 / 3) * tangent
    q2 = p1 - (1 / 3) * tangent
    q3 = p1
    return np.array([q0, q1, q2, q3])

def bezier_curve(points, tangents):
    segments = []
    for j in range(len(points) - 1):
        segments.append(bezier_points(points[j], points[j+1], tangents[j]))
    return segments

def evaluate(segment, t):
    q0, q1, q2, q3 = segment
    return ((1 - t)**3)[:, None] * q0 + \
           (3 * (1 - t)**2 * t)[:, None] * q1 + \
           (3 * (1 - t) * t**2)[:, None] * q2 + \
           (t**3)[:, None] * q3

def plot_22(m1):
    heart_points = heart_curve(200)

    plt.figure(figsize=(8, 8))
    plt.plot(heart_points[:, 0], heart_points[:, 1], 'k-', label="True Heart")

    for m in m1:
        points = heart_curve(m+1)
        tangents = Tangents(points)

        segments = bezier_curve(points, tangents)

        points_all = np.vstack([
            evaluate(segment, np.linspace(0, 1, 100))
            for segment in segments
        ])

        plt.plot(points_all[:, 0], points_all[:, 1], label=f'Bezier m={m}')

    plt.xlabel('x')
    plt.ylabel('y')
    plt.title("Heart Curve and Bezier Approximations")
    plt.legend()
    plt.grid(True)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

m1 = [10, 40, 160]
plot_22(m1)
