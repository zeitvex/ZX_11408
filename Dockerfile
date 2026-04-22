# --- 构建阶段 ---
FROM node:18-alpine AS build-stage
WORKDIR /app

# 设置 npm 镜像源 (加速下载)
RUN npm config set registry https://registry.npmmirror.com

# 复制依赖文件并安装
COPY package*.json ./
RUN npm install

# 复制项目所有文件
COPY . .

# 执行 VitePress 构建
RUN npm run build

# --- 运行阶段 ---
FROM nginx:stable-alpine AS production-stage

# 将编译产物复制到 Nginx 目录
COPY --from=build-stage /app/.vitepress/dist /usr/share/nginx/html

# 复制自定义 Nginx 配置 (可选，支持 SPA 路由)
RUN printf "server { \n\
    listen 80; \n\
    location / { \n\
        root /usr/share/nginx/html; \n\
        index index.html index.htm; \n\
        try_files \$uri \$uri.html \$uri/ =404; \n\
    } \n\
}" > /etc/nginx/conf.d/default.conf

EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
