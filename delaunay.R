library(deldir)
input = read.table("~/Documents/Harsh/spring2017/mwt/report/input2/input2.txt")

x <- input$V1
y <- input$V2
vtess <- deldir(x,y)

plot(x,y,type="n",asp=1)
plot(vtess, wlines="triang",wpoints="none",number=FALSE,add=TRUE,lty=1)
points(x,y,pch=20,col="blue",cex=0.5)

size = length(vtess$delsgs$x1)
cost = 0

for (i in 1:size)
{
  squared_distance = (vtess$delsgs$x1[i]-vtess$delsgs$x2[i])^2 + (vtess$delsgs$y1[i]-vtess$delsgs$y2[i])^2
  cost = cost + sqrt(squared_distance)
}