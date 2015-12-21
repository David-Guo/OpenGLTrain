# 开发日志

使用 openGL stencil test 产生阴影效果。

最终结果如下所示：

![](./picture/3.png)

## bug 及其解决

### scene 1 问题

* shadow polygon 产生问题

出现如下图所示的 bug :

![](./picture/2.png)

原因是在进行 stencil test 时，`glStencilOp` 调用顺序不对，下面是错误的方式：

```c
// 锁定 color buffer and depth buffer
glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
glDepthMask(GL_FALSE);
glEnable(GL_CULL_FACE);	
// pass 2. front face stencil update
glCullFace(GL_BACK);						// 切掉 back face
shadowPolygon();
glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);		// 通过z-buff 测试，stencil 值+1
// pass 3. back face stencil update
glCullFace(GL_FRONT);						// 切掉 front face	
shadowPolygon();
glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);		// 通过z-buff 测试，stencil 值-1
// 恢复
glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
glDepthMask(GL_TRUE); 
glStencilMask(~0);							// 锁定 stencile 
glDisable(GL_CULL_FACE);
// pass 4. 
glStencilFunc(GL_EQUAL, 0, ~0);
glDepthFunc(GL_EQUAL); glBlendFunc(GL_ONE, GL_ONE);
light();
drawScene();
```

如果在画出 shadow polygon 之前就调用 `glStenciOp` 那么就无法，将 `glStenciOp` 作用到 shadow polygon 上。


* 按下键盘，触发移动视点事件时，出现下面的 bug 

![](./picture/4.png)

此时需要在上面代码的后面加上如下两句：

```c
glDisable(GL_STENCIL_TEST);
glStencilFunc(GL_ALWAYS, 0, ~0);
```

* 阴影颜色太深问题 

![](./picture/5.png)

主要是直接把lighting disable

正确做法是只把diffuse與specular光源亮度设为 0 0 0

所以需要添加一个 `ambient_lignt()` 函数：

```
void ambient_light()
{
	glShadeModel(GL_SMOOTH);
	// z buffer enable
	glEnable(GL_DEPTH_TEST);
	// enable lighting
	glEnable(GL_LIGHTING);
	float diffuse[3] = {0, 0, 0};
	float specular[3] = {0, 0, 0};

	for (size_t i = 0; i < globalight->lTotal; i++) {
		// set light property
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, globalight->lightList[i].light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, globalight->lightList[i].light_ambient);
	}
	//glutSolidCube(1);
}
```

### scene 2 问题

* 有体积物体无法产生阴影问题

![](./picture/6.png)


原因是：平板是单面的，画平板是不需要画背光面。而有体积的物体是封闭的，一定会有背光面。
如果没有特别区分背光面与正光面，背光面的 shadow volume 与正光面的 shadow volume 计算 stencil count 会相互抵消。

正确做法是，使用 face 上任意一点的法向量与该点到光源的向量做内积，结果大于零表示是正光面，此时作图顺序是 AA'B'B。（A'B' 表示自动生成的远光点）

结果小于零表示为背光面，作图顺序是 ABB'A'。

效果如下图所示：

![](./picture/7.png)



