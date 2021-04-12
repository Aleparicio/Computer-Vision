from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay
import matplotlib.pyplot as plt 

labels = ['circulo', 'triangulo', 'vagon', 'rectangulo', 'rueda', 'desconocido']

def plot_confusion_matrix(ytest, ypred):
    cm = confusion_matrix(ytest, ypred, labels=labels)
    cmd = ConfusionMatrixDisplay(cm, display_labels=labels)
    cmd.plot()
    plt.show()

# reco1
ytest = ['circulo', 'triangulo', 'vagon', 'rueda']
ypred_c = ['circulo', 'triangulo', 'desconocido', 'rueda']
plot_confusion_matrix(ytest, ypred_c)
ypred_e = ['circulo', 'triangulo', 'vagon', 'rueda']
plot_confusion_matrix(ytest, ypred_e)

# reco 2
# y[0] = rueda + triángulo
ytest = ['desconocido', 'vagon', 'circulo', 'rectangulo']
ypred_c = ['desconocido', 'desconocido', 'circulo', 'desconocido']
# En esta salen muchos desconocidos, mirar los números
plot_confusion_matrix(ytest, ypred_c)
ypred_e = ['desconocido', 'vagon', 'circulo', 'rectangulo']
plot_confusion_matrix(ytest, ypred_e)

# reco 3
ytest = ['circulo', 'triangulo', 'rueda', 'vagon']
ypred_c = ['circulo', 'triangulo', 'rueda', 'desconocido']
plot_confusion_matrix(ytest, ypred_c)
ypred_e = ['circulo', 'triangulo', 'rueda', 'desconocido']
plot_confusion_matrix(ytest, ypred_e)
