import cherrypy
import twitter
import csv

secretKeys = {'con_key':[],'con_secret':[],'acc_token':[], 'acc_token_secret':[]}

inf = open('.secret.key')
keys = csv.reader(inf, delimiter = ',') 
for row in keys:
	secretKeys['con_key']=row[0]
	secretKeys['con_secret']=row[1]
	secretKeys['acc_token']=row[2]
	secretKeys['acc_token_secret']=row[3]



inf.close()


api = twitter.Api(consumer_key=secretKeys['con_key'],
		  consumer_secret=secretKeys['con_secret'],
		  access_token_key=secretKeys['acc_token'],
		  access_token_secret=secretKeys['acc_token_secret'])

class Root(object):
    @cherrypy.expose
    def index(self, **params):
	print("Message recieved from RockBlock: " + params['imei'])
	print("Message time: " + params['transmit_time'])
	message = params['data'].decode("hex")
	print("Message: " + message)
	tweet = "Current RockBlock Time is "+params['transmit_time'] + '. \n' + 'Current Message: ' + message
	status = api.PostUpdate(tweet)
	return ""

if __name__ == '__main__':
   cherrypy.config.update("twitterBotConfig.conf")
   cherrypy.quickstart(Root())
