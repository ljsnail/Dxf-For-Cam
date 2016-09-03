this project aim to create the Cam function from a dxf file ,while is the result of nest.
The project would including some part as follow :
1.the data struct of the dxf 
2.the algorithm of the process from dxf to Cam .
3.create a tool path and optimize
4.output a G-code
5........



As you can see ,the project is not complete,and everyday ,I would git add and git push .
If you also doing  or had done something like this,welcome to make some contribution to it .
If you find something useful from my repository,you can take it into your project ,but please do not use it in commerce project
2016.8.13完成了用双向链表保存所有原始数据的工作
接下来是对数据进行处理了
20.16.8.15对数据处理重新改写，原本是统一先挂进来，然后彼此之间前后指向，现在改为每增加一个结点就判断其与原来数据结点的关系
，特别对圆这种本事就是封闭环的结点，采用了，一开始就直接把它独立到另一个封闭环里。并不与其他结点挂在一条双向链上。