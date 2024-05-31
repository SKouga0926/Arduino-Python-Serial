def calculate_zmp(points, forces):
    # 各点のx座標とy座標を抽出
    x_coords = [point[0] for point in points]
    y_coords = [point[1] for point in points]
    
    # 合力を計算
    total_force = sum(forces)
    
    # 合力に対する各点の重み付き座標の合計を計算
    weighted_sum_x = sum(x * force for x, force in zip(x_coords, forces))
    weighted_sum_y = sum(y * force for y, force in zip(y_coords, forces))
    
    # ZMPのx座標とy座標を計算
    zmp_x = weighted_sum_x / total_force
    zmp_y = weighted_sum_y / total_force
    
    return zmp_x, zmp_y

# 座標点とそれぞれの点にかかる力のリストを定義
points = [(0, 57), (57, 43), (0, 0), (57, 0)]
forces = [0, 50, 20, 0]

# ZMPを計算
zmp_x, zmp_y = calculate_zmp(points, forces)

print("ZMPのx座標:", zmp_x)
print("ZMPのy座標:", zmp_y)
